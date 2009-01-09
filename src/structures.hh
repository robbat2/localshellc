#ifndef _STRUCTURES_HH_
#define _STRUCTURES_HH_

#include <features.h>
#include <stddef.h>
#include <pwd.h> //passwd
#include <grp.h> //group
//#include <sys/types.h> // for uid_t, gid_t
//
#include <limits.h> // INT_MAX

#include <map>
#include <vector>
using namespace std;

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

typedef map<int,cfg_entry*> cfg_entries_map;
typedef vector<cfg_list*> cfg_list_vector;

struct configuration {
	cfg_entries_map cfg_entries;
	cfg_list_vector preferred_shell_files;
	cfg_list_vector preferred_shells;
	cfg_list_vector default_shells;
};

#endif //_STRUCTURES_HH_
