#ifndef _CONFIG_HH_
#define _CONFIG_HH_
#include "structures.hh"
#include <fstream>

int load_config( const char *cfg_filename, configuration &conf);
int parse_config(fstream &fs, configuration &conf);
int parse_config_argument(const char *argname, uid_t uid, gid_t gid, const char *argvalue, configuration &conf);
int parse_config_argument__entry(uid_t uid, gid_t gid, const char *argvalue, configuration &conf);
int parse_config_argument__preferred_shell_file(uid_t uid, gid_t gid, const char *argvalue, configuration &conf);
int parse_config_argument__default_preferred_shell(uid_t uid, gid_t gid, const char *argvalue, configuration &conf);
int parse_config_argument__default_shell(uid_t uid, gid_t gid, const char *argvalue, configuration &conf);
cfg_list* create_cfg_list(const char *value, uid_t uid, gid_t gid);
#endif //_CONFIG_HH_

