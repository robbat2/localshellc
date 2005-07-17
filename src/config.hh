/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/config.hh,v 1.2 2005/07/17 19:57:56 robbat2 Exp $ */
#ifndef _CONFIG_HH_
#define _CONFIG_HH_
#include "structures.hh"
int load_config( const char *cfg_filename, configuration &conf);
int parse_config(fstream &fs, configuration &conf);
gid_t resolve_gid(char *group);
uid_t resolve_uid(char *user);
int parse_config_argument(char *argname, uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__entry(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__preferred_shell_file(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__default_preferred_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
int parse_config_argument__default_shell(uid_t uid, gid_t gid, char *argvalue, configuration &conf);
#endif //_CONFIG_HH_

