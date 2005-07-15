#ifndef _CONFIG_HH_
#define _CONFIG_HH_

#include <map>
#include <vector>
using namespace std;

int parse_config( const char *cfg_filename, map<int,cfg_entry> &cfg_entries, vector<cfg_list> &preferred_shell_files, vector<cfg_list> &preferred_shells, vector<cfg_list> &default_shells);

#endif //_CONFIG_HH_

