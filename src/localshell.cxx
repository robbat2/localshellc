/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/localshell.cxx,v 1.2 2005/07/17 19:57:56 robbat2 Exp $ */

#include <stdio.h>
#include "structures.hh"
#include "config.hh"
#include "common.hh"

#include <map>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
	if(argc > 0) {
		configuration conf;
		load_config(argv[1],conf);
		printf("%d\n",hash_string(argv[1]));
	}
	return 0; /* NOT-REACHED */
}
