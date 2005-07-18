/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/common.cxx,v 1.1 2005/07/18 01:03:50 robbat2 Exp $ */

#include <stdio.h>
#include <errno.h>
#include "structures.hh"
#include "common.hh"
#include <fstream>
using namespace std;

bool match_user(uid_t uid, gid_t gid) {
	bool uid_match = false;
	bool gid_match = false;

	uid_match = (uid == UID_ANY) || (uid == getuid());
	gid_match = (gid == GID_ANY) || (gid == getgid());

	if(!gid_match) {
		errno = 0;
		long groups_max = 32; //32 * 4 bytes = 128 bytes
		gid_t *grouplist = NULL;
		int count;
		// we do this as NGROUPS_MAX is 64K, and we really don't want to eat 256K of heap.
		// on most systems/cases this loop will run exactly once, with no penalties.
		do {
			// ok, so we need more space
			// clean up and try again
			if(NULL != grouplist) { 
				groups_max *= 2; 
				delete grouplist;
			}
			grouplist = new gid_t[groups_max];
			count = getgroups(groups_max,grouplist);
		} while (errno == EINVAL);
		// now we can try and match the data
		for(int i = 0; i < count; i++) {
			if(gid == grouplist[i]) { 
				gid_match = true; 
				break; 
			}
		}
		delete[] grouplist;
	}
	return uid_match && gid_match;
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

char* get_preferred_shell(char* filename) {
	const int BUFFERSIZE = 2048; // compiles away
	char line[BUFFERSIZE];
	memset(line,'\0',BUFFERSIZE); // set to NULL for safety
	// get the first line
	fstream fs(filename, ios_base::in);
	char* result;
	if(fs.is_open() && fs.good()) {
		fs.getline(line,BUFFERSIZE);
		result = strndup(line,BUFFERSIZE);
	} else {
		result = NULL;
	}
	fs.close();
	return result;
}

char* get_preferred_shell(configuration &conf) {
	char* shell = NULL;
	for(cfg_list_vector::iterator it = conf.preferred_shell_files.begin(); it != conf.preferred_shell_files.end() && shell == NULL; it++) {
		shell = get_preferred_shell((*it)->value);
		// if no access, block this shell
		if(NULL != shell && !test_shell(shell)) {
			shell = NULL;
		}
	}
	for(cfg_list_vector::iterator it = conf.preferred_shells.begin(); it != conf.preferred_shells.end() && shell == NULL; it++) {
		shell = (*it)->value;
		// if no access, block this shell
		if(NULL != shell && !test_shell(shell)) {
			shell = NULL;
		}
	}
	return shell;
}
char* get_default_shell(configuration &conf) {
	char* shell = NULL;
	for(cfg_list_vector::iterator it = conf.default_shells.begin(); it != conf.default_shells.end() && shell == NULL; it++) {
		shell = (*it)->value;
		// if no access, block this shell
		if(NULL != shell && !test_shell(shell)) {
			shell = NULL;
		}
	}
	return shell;
}

bool test_shell(char* shell) {
	return 0 == access(shell,X_OK);
}
