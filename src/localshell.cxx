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
	/*
	fprintf(stderr,"Initial run %s with",argv[0]);
	for(int i = 0; i < argc; i++) {
		fprintf(stderr," %d='%s'",i,argv[i]);
	}
	fprintf(stderr,"\n");
	*/
	//printf("argc: %d\n",argc);
	if(load_config(CONFIGFILE,conf)) {
		// We failed.
		fprintf(stderr,"%s: Cannot load config\n", PACKAGE);
		return 1;
	}
	const int BUFFERSIZE = 4096;	
	int start = 1;
	int first = 1;
	char argstring[BUFFERSIZE];
	int space = BUFFERSIZE;
	memset(argstring,'\0',BUFFERSIZE);
	if(argc > 1 && strcmp(argv[1], "-c") == 0) {
		start = 2;
	}
	for(int i = start; i < argc; i++) {
		if(first) first = 0; else strncat(argstring," ",space--);
		strncat(argstring,argv[i],space);
		space -= strlen(argv[i]);
	}
	//printf("Args: '%s'\n",argstring);

	for( cfg_entries_map::reverse_iterator it = conf.cfg_entries.rbegin();  
			it != conf.cfg_entries.rend(); 
			it++) {
		cfg_entry *c = (*it).second;
		//fprintf(stderr,"%d %d:%d - %s - %s\n", c->priority, c->uid, c->gid, c->shell, c->allowed_cmds);
		// Now is final pass checking
		// 1. check execute rights to shell
		if(0 == strcmp(c->shell,"LOCALSHELL")) {
			c->shell = get_preferred_shell(conf);
		} else if(0 == strcmp(c->shell,"DEFAULTSHELL")) {
			c->shell = get_default_shell(conf);
		}
		// We would have printed a warning
		if(c->shell == NULL) {
			fprintf(stderr,"%s: Shell is NULL\n", PACKAGE);
			continue;
		}
		// 2. check allowed_commands is good
		//fprintf(stderr,"Checking argstring(%s) against (%s)\n",argstring,c->allowed_cmds);
		int status;
		regex_t re;
		if (0 != regcomp(&re, c->allowed_cmds, REG_EXTENDED|REG_NOSUB)) {
			fprintf(stderr,"%s:Failed to compile regex (%s)\n", PACKAGE, c->allowed_cmds);
			continue;
		}
		status = regexec(&re, argstring, (size_t) 0, NULL, 0);
		regfree(&re);
		// 3. run execv
		if(0 == status) {
			int newargc;
			//fprintf(stderr,"%d %d:%d - %s - %s\n", c->priority, c->uid, c->gid, c->shell, c->allowed_cmds);
			memset(argstring,'\0',BUFFERSIZE);
			if(argc == 1) {
				argstring[0] = '-';
				newargc = argc;
			} else {
				newargc = argc-start+2;
				char** newargv = new char*[newargc];
				for(int i = argc; i >= start  ; i-- ) {
					//printf("argv[%i]:'%s'\n",i,argv[i]);
					newargv[i-start+2] = argv[i];
				}
				newargv[1] = "-c";
				argv = newargv;
			}
			strncat(argstring,c->shell,BUFFERSIZE-1);
			argv[0] = argstring;
			// probably won't return from this
			/*
			fprintf(stderr,"Running %s with",c->shell);
			for(int i = 0; i < newargc; i++) {
				fprintf(stderr," %d='%s'",i,argv[i]);
			}
			fprintf(stderr,"\n");
			*/
			execv(c->shell,argv);
			int err = errno;
			fprintf(stderr,"%s: failed to launch shell (errno:%d)\n",err);
		}
	}
	return 1; // only reach here in case of error
}
