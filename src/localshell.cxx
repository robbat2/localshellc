/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/localshell.cxx,v 1.5 2005/07/18 20:57:21 robbat2 Exp $ */

#include <stdio.h>
#include "structures.hh"
#include "config.hh"
#include "common.hh"
#include "built-configfile.h"
#include <regex.h>
#include <errno.h>

#include <map>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
	configuration conf;
	//printf("argc: %d\n",argc);
	if(0 == load_config(CONFIGFILE,conf)) {
		const int BUFFERSIZE = 4096;	
		char argstring[BUFFERSIZE];
		int space = BUFFERSIZE;
		memset(argstring,'\0',BUFFERSIZE);
		for(int i = 1; i < argc; i++) {
			if(i != 1) strncat(argstring," ",space--);
			strncat(argstring,argv[i],space);
			space -= strlen(argv[i]);
		}
		//printf("Args: '%s'\n",argstring);

		for( cfg_entries_map::reverse_iterator it = conf.cfg_entries.rbegin();  it != conf.cfg_entries.rend(); it++) {
			cfg_entry *c = (*it).second;
			// Now is final pass checking
			// 1. check execute rights to shell
			if(0 == strcmp(c->shell,"LOCALSHELL")) {
				c->shell = get_preferred_shell(conf);
			} else if(0 == strcmp(c->shell,"DEFAULTSHELL")) {
				c->shell = get_default_shell(conf);
			}
			// We would have printed a warning
			if(c->shell == NULL) continue;
			// 2. check allowed_commands is good
			int status;
			regex_t re;
			if (0 != regcomp(&re, c->allowed_cmds, REG_EXTENDED|REG_NOSUB)) continue;
			status = regexec(&re, argstring, (size_t) 0, NULL, 0);
			regfree(&re);
			// 3. run execv
			if(0 == status) {
				//fprintf(stderr,"%d %d:%d - %s - %s\n", c->priority, c->uid, c->gid, c->shell, c->allowed_cmds);
				memset(argstring,'\0',4);
				if(argc == 1) {
					argstring[0] = '-';
				} else {
					char** newargv = new char*[argc+1];
					for(int i = argc ; i > 0 ; i-- ) {
						//printf("argv[%i]:'%s'\n",i,argv[i]);
						newargv[i+1] = argv[i];
					}
					newargv[1] = "-c";
					argv = newargv;
				}
				strncat(argstring,c->shell,BUFFERSIZE-1);
				argv[0] = argstring;
				// probably won't return from this
				execv(c->shell,argv);
				int err = errno;
				fprintf(stderr,"%s: failed to launch shell (errno:%d)\n",err);
			}
		}
	}
	return 1; // only reach here in case of error
}
