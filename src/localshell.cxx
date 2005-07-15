/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/localshell.cxx,v 1.1 2005/07/15 02:03:38 robbat2 Exp $ */

#include <stdio.h>
#include "structures.hh"
#include "config.hh"

#include <map>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
	map<int,cfg_entry> cfg_entries;
	vector<cfg_list> preferred_shell_files;
	vector<cfg_list> preferred_shells;
	vector<cfg_list> default_shells;

	parse_config(argv[1],cfg_entries,preferred_shell_files,preferred_shells,default_shells);
	return 0; /* NOT-REACHED */
}
