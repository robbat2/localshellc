/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/config.hh,v 1.3 2005/07/18 01:03:50 robbat2 Exp $ */
#ifndef _CONFIG_HH_
#define _CONFIG_HH_
#include "structures.hh"

#ifndef CONFIG_FILE
#define CONFIG_FILE "/etc/localshell.conf"
#endif

int load_config( const char *cfg_filename, configuration &conf);
int parse_config(fstream &fs, configuration &conf);
int parse_config_argument(char *argname, uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__entry(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__preferred_shell_file(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__default_preferred_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__default_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
cfg_list* create_cfg_list(char *value, uid_t uid, gid_t gid);
#endif //_CONFIG_HH_

