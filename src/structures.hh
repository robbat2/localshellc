/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/structures.hh,v 1.1 2005/07/15 02:03:38 robbat2 Exp $ */
#ifndef _STRUCTURES_HH_
#define _STRUCTURES_HH_

#include <features.h>
#include <stddef.h>
#include <pwd.h> //passwd
#include <grp.h> //group
//#include <sys/types.h> // for uid_t, gid_t
//
#include <limits.h> // INT_MAX
#include "../config.h"

const uid_t UID_INVALID = INT_MAX;
const gid_t GID_INVALID = INT_MAX;
const uid_t UID_ANY = UID_INVALID-1;
const gid_t GID_ANY = UID_INVALID-1;

struct cfg_entry {
	signed int priority; // KEY
	char* shell;
	char* allowed_cmds;
	// these are used during loading only
	uid_t uid; 
	gid_t gid; 
};

// generic config stuff for preferred_shell_file/default_preferred_shell/default_shell
struct cfg_list {
	char *value;
	// these are used during loading only
	uid_t uid; 
	gid_t gid; 
};

#endif //_STRUCTURES_HH_
