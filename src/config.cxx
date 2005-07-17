/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/config.cxx,v 1.2 2005/07/17 19:57:56 robbat2 Exp $ */

#include <stdio.h>
#include <errno.h>
#include "structures.hh"
#include "config.hh"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
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

int parse_config(fstream &fs, configuration &conf) {
	const int BUFFERSIZE = 2048; // compiles away
	int len = BUFFERSIZE;
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
		char* colon_pos = strstr(equal_pos+2,":");
		if(NULL == colon_pos) continue;
		// now the ,
		char* comma_pos = strstr(colon_pos+1,",");
		if(NULL == comma_pos) continue;
		// now the trailing "
		char* trailing_quote_pos = strstr(comma_pos+1,"\"");
		if(NULL == trailing_quote_pos) continue;

		char *argname = line;
		char *arguid = equal_pos+2;
		char *arggid = colon_pos+1;
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
	
		printf("Name:'%s' UID:%d GID:%d Value:'%s'\n",argname,uid,gid,argvalue);
		if(match_user(uid,gid)) {
			parse_config_argument(argname,uid,gid,argvalue,conf);
		}
	}
}

uid_t resolve_uid(char *user) {
	uid_t uid = UID_INVALID;
	if(0 == strlen(user)) {
		uid = UID_ANY;
	} else {
		int c = sscanf(user,"%d",&uid);
		// didn't find a numeric uid, assume string and do lookup
		passwd *pwd;
		if(1 != c) {
			errno = 0;
			pwd = getpwnam(user);
			// does not exist, bypass
			if(NULL == pwd) {
				fprintf(stderr,"%s: bad user:'%s'\n",PACKAGE,user);
				if(0 == errno) errno = EINVAL;
			} else {
				uid = pwd->pw_uid;
			}
		}
	}
	return uid;
}

gid_t resolve_gid(char *groupname) {
	gid_t gid = GID_INVALID;
	if(0 == strlen(groupname)) {
		gid = GID_ANY;
	} else {
		int c = sscanf(groupname,"%d",&gid);
		// didn't find a numeric gid, assume string and do lookup
		group *grp;
		if(1 != c) {
			errno = 0;
			grp = getgrnam(groupname);
			// does not exist, bypass
			if(NULL == grp) {
				fprintf(stderr,"%s: bad group:'%s'\n",PACKAGE,groupname);
				if(0 == errno) errno = EINVAL;
			} else {
				gid = grp->gr_gid;
			}
		}
	}
	return gid;
}

int parse_config_argument(char *argname, uid_t uid, gid_t gid, char *argvalue, configuration &conf) {
	int ret = EINVAL;
	switch(hash_string(argname)) {
		case 80466: 
			/* entry */ 
			ret = parse_config_argument__entry(uid,gid,argvalue,conf);
			break;
		case 1879921805:
			/* preferred_shell_file */ 
			ret = parse_config_argument__preferred_shell_file(uid,gid,argvalue,conf);
			break;
		case -1144786314:
			/* default_preferred_shell */ 
			ret = parse_config_argument__default_preferred_shell(uid,gid,argvalue,conf);
			break;
		case 522816976:
			/* default_shell */ 
			ret = parse_config_argument__default_shell(uid,gid,argvalue,conf);
			break;
		default:
			/* all other cases */
			fprintf(stderr,"%s: bad configuration argument:'%s'\n",PACKAGE,argname);
			break;
	}
	return ret;
}

bool match_user(uid_t uid, gid_t gid) {
	if(uid == getuid() || gid == getgid()) {
		return true;
	} else {
		// now check supplementary groups
		group *grp;
		grp = getgrgid(gid);
		for( char* username = grp->gr_mem ; grp->gr_mem += sizeof(char *) ; grp->gr_mem != NULL ) {
			uid_t uid2 = resolve_uid(username);
			if(uid2 == uid) return true;
		}
	}
}

int parse_config_argument__entry(uid_t uid, gid_t gid, char *argvalue, configuration &conf) {
	cfg_entry *entry = new cfg_entry;
	// PRIORITY,SHELL,ALLOWED
	char* priority = argvalue;

	char* shell = strstr(argvalue,",");
	if(NULL == shell) { /* error */ }
	shell[0] = '\0'; shell++;

	char* allowed_cmds = strstr(shell,",");
	if(NULL == allowed_cmds) { /* error */ }
	allowed_cmds[0] = '\0'; allowed_cmds++;

	entry->priority = priority;
	entry->shell = strdup(shell);
	entry->allowed_cmds = strdup(allowed_cmds);
	entry->uid = uid;
	entry->gid = gid;
	conf.cfg_entries[priority] = entry;
}
int parse_config_argument__preferred_shell_file(uid_t uid, gid_t gid, char *argvalue, configuration &conf) {
	conf.preferred_shell_files.push_bash(create_cfg_list(argvalue,uid,gid));
}
int parse_config_argument__default_preferred_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf) {
	conf.preferred_shells.push_bash(create_cfg_list(argvalue,uid,gid));
}
int parse_config_argument__default_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf) {
	conf.default_shells.push_bash(create_cfg_list(argvalue,uid,gid));
}
