#include <stdio.h>
#include <errno.h>
#include "structures.hh"
#include "common.hh"
#include "config.hh"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

int load_config( const char *cfg_filename, configuration &conf) {
	fstream fs(cfg_filename, ios_base::in);
	if(!fs.is_open()) {
		fprintf(stderr,"%s: error opening config file\n",PACKAGE);
		return ENOENT;
	}
	int ret = parse_config(fs,conf);
	fs.close(); // very important for a shell-spawner
	return ret;
}
				
void load_defaults(configuration &conf) {
#define LOAD_DEFAULT(setting, var) \
	if(parse_config_argument(setting, UID_ANY, GID_ANY, var, conf)) { \
		fprintf(stderr, "Bad default config %s=%s %d:%d\n", setting, var, UID_ANY, GID_ANY); \
	}
	LOAD_DEFAULT("preferred_shell_file", "~/.localshellrc");
	LOAD_DEFAULT("default_preferred_shell", "/bin/bash");
	LOAD_DEFAULT("default_shell", "/bin/false");
#undef LOAD_DEFAULT
}

int parse_config(fstream &fs, configuration &conf) {
	const int BUFFERSIZE = 2048; // compiles away
	int len = BUFFERSIZE;
	/* Load defaults */
	load_defaults(conf);
	/* Now check the config */
	while(fs.good()) {
		char line[BUFFERSIZE];
		memset(line,'\0',len); // set to NULL for safety
		fs.getline(line,BUFFERSIZE);

		// if this line is a comment, skip it
		if('#' == line[0]) continue;
		// or if the line is empty
		len = strlen(line);
		if(0 == len) continue;

		// find markers and store their position
		// first the =
		char* equal_pos = strstr(line,"=");
		if(NULL == equal_pos) continue;
		// now the :
		char* colon_pos = strstr(equal_pos,":");
		// now the ,
		char* comma_pos = strstr(equal_pos,",");
		if(NULL == comma_pos || ((colon_pos != NULL) && (comma_pos < colon_pos))) continue;
		if(NULL == colon_pos) { colon_pos = comma_pos; }
		// now the trailing "
		char* trailing_quote_pos = strstr(comma_pos,"\"");
		if(NULL == trailing_quote_pos) continue;

		char *argname = line;
		char *arguid = equal_pos+2;
		char *arggid = colon_pos == comma_pos ? colon_pos : colon_pos+1;
		char *argvalue = comma_pos+1;
		// this is an evil trick, so we don't need any seperate buffers or copying
		// basically the single buffer contains everything...
		equal_pos[0] = equal_pos[1] = colon_pos[0] = comma_pos[0] = trailing_quote_pos[0] = '\0';

		uid_t uid;
		gid_t gid;

		// now check uid/gid for being numbers
		uid = resolve_uid(arguid);
		if(UID_INVALID == uid) return EINVAL;
		gid = resolve_gid(arggid);
		if(GID_INVALID == gid) return EINVAL;
	
		//printf("Name:'%s' UID:%d GID:%d Value:'%s'\n",argname,uid,gid,argvalue);
		if(match_user(uid,gid)) {
			if(parse_config_argument(argname,uid,gid,argvalue,conf)) {
				fprintf(stderr, "Bad config %s=%s %d:%d\n", argname, argvalue, uid, gid);
			}
		}
	}
	return 0;
}

int parse_config_argument(const char *argname, uid_t uid, gid_t gid, const char *argvalue, configuration &conf) {
	int ret = EINVAL;
	int match = 0;
	//printf("Adding %s:\n",argname);
#define PARSE(type) \
	if(match == 0 && 0 == strcmp(argname, #type)) { \
		/*fprintf(stderr, "Processing %s=%s %d:%d\n", #type, argvalue, uid, gid);*/ \
		ret = parse_config_argument__##type (uid,gid,argvalue,conf); \
		match = 1; \
	}
	PARSE(entry);
	PARSE(preferred_shell_file);
	PARSE(default_preferred_shell);
	PARSE(default_shell);
#undef PARSE

	if(match == 0) {
		// all other cases 
		return EINVAL;
	}
	
	return ret;
}

int parse_config_argument__entry(uid_t uid, gid_t gid, const char *argvalue, configuration &conf) {
	cfg_entry *entry = new cfg_entry;
	// PRIORITY,SHELL,ALLOWED
	long priority = atol(argvalue);

	char* shell = const_cast<char*>(strstr(argvalue,","));
	if(NULL == shell || '\0' == shell[1] ) { 
		fprintf(stderr,"%s: badly formatted entry (shell):%s\n",PACKAGE,argvalue); 
		return EINVAL; 
	}
	shell[0] = '\0'; shell++;
	char* allowed_cmds = const_cast<char*>(strstr(shell,","));
	if(NULL == allowed_cmds) {
		fprintf(stderr,"%s: badly formatted entry (allowed_cmds):%s\n",PACKAGE,shell); 
		return EINVAL; 
	}
	allowed_cmds[0] = '\0'; allowed_cmds++;
	
	if(strcmp("DEFAULTSHELL", shell) && 
			strcmp("LOCALSHELL", shell) && 
			!test_shell(shell)) {
		fprintf(stderr,"%s: invalid shell:%s\n", PACKAGE, shell);
		return EINVAL; 
	}

	entry->priority = priority;
	entry->shell = strdup(shell);
	entry->allowed_cmds = strdup(allowed_cmds);
	entry->uid = uid;
	entry->gid = gid;
	conf.cfg_entries[priority] = entry;
			
	//printf("%d %d:%d - %s - %s\n", entry->priority, entry->uid, entry->gid, entry->shell, entry->allowed_cmds);
	return 0;
}
int parse_config_argument__preferred_shell_file(uid_t uid, gid_t gid, const char *argvalue, configuration &conf) {
	conf.preferred_shell_files.push_back(create_cfg_list(argvalue,uid,gid));
	return 0;
}
int parse_config_argument__default_preferred_shell(uid_t uid, gid_t gid, const char *argvalue, configuration &conf) {
	conf.preferred_shells.push_back(create_cfg_list(argvalue,uid,gid));
	return 0;
}
int parse_config_argument__default_shell(uid_t uid, gid_t gid, const char *argvalue, configuration &conf) {
	conf.default_shells.push_back(create_cfg_list(argvalue,uid,gid));
	return 0;
}
cfg_list* create_cfg_list(const char *value, uid_t uid, gid_t gid) {
	cfg_list *cfg = new cfg_list;
	cfg->value = strdup(value);
	cfg->uid = uid;
	cfg->gid = gid;
	return cfg;
}

