/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/config.cxx,v 1.1 2005/07/15 02:03:38 robbat2 Exp $ */

#include <stdio.h>
#include <errno.h>
#include "structures.hh"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

int load_config( const char *cfg_filename, map<int,cfg_entry> &cfg_entries, vector<cfg_list> &preferred_shell_files, vector<cfg_list> &preferred_shells, vector<cfg_list> &default_shells) {
	fstream fs(cfg_filename, ios_base::in);
	if(!fs.is_open()) {
		cerr << PACKAGE << ": error opening config file" << endl;
		return ENOENT;
	}
	int ret = parse_config(fs,cfg_entries,preferred_shell_files,preferred_shells,default_shells);
	fs.close(); // very important for a shell-spawner
	return ret;
}

int parse_config(fstream &fs, map<int,cfg_entry> &cfg_entries, vector<cfg_list> &preferred_shell_files, vector<cfg_list> &preferred_shells, vector<cfg_list> &default_shells) {
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

		char argname[BUFFERSIZE];
		char arguid[BUFFERSIZE];
		char arggid[BUFFERSIZE];
		char argvalue[BUFFERSIZE];

		memset(argname,'\0',len);
		memset(arguid,'\0',len);
		memset(arggid,'\0',len);
		memset(argvalue,'\0',len);

		// copy the argument
		strncpy(argname,line,equal_pos - line);
		strncpy(arguid,equal_pos+2,colon_pos - (equal_pos + 2));
		strncpy(arggid,colon_pos+1,comma_pos - (colon_pos + 1));
		strncpy(argvalue,comma_pos+1,trailing_quote_pos - (comma_pos + 1));

		printf("Name:'%25s' UID:'%25s' GID:'%25s' Value:'%25s'\n",argname,arguid,arggid,argvalue);

		uid_t uid;
		gid_t gid;

		// now check uid/gid for being numbers
		uid = resolve_uid(arguid);
		if(UID_INVALID == uid) return EINVAL;
		gid = resolve_gid(arggid);
		if(GID_INVALID == gid) return EINVAL;

		// uid/gid done
	}
}

uid_t resolve_uid(char *user) {
	uid_t = UID_INVALID;
	if(0 == strlen(user)) {
		uid = UID_ANY;
	} else {
		int c = scanf(user,"%d",&uid);
		// didn't find a numeric uid, assume string and do lookup
		if(1 != c) {
			passwd *pwd;
			errno = 0;
			pwd = getpwnam(user);
			// does not exist, bypass
			if(NULL == pwd) {
				cerr << PACKAGE << ": bad user:'" << group << "'" << endl;
				if(0 == errno) errno = EINVAL;
			} else {
				uid = pwd->pw_uid;
			}
		}
	}
	return uid;
}

gid_t resolve_gid(char *group) {
	gid_t = GID_INVALID;
	if(0 == strlen(group)) {
		gid = GID_ANY;
	} else {
		int c = scanf(group,"%d",&gid);
		// didn't find a numeric gid, assume string and do lookup
		if(1 != c) {
			group *grp;
			errno = 0;
			grp = getgrnam(group);
			// does not exist, bypass
			if(NULL == grp) {
				cerr << PACKAGE << ": bad group:'" << group << "'" << endl;
				if(0 == errno) errno = EINVAL;
			} else {
				gid = grp->gr_gid;
			}
		}
	}
	return gid;
}
