# Copyright (C) 2005 Robin Hugh Johnson <robbat2@orbis-terrarum.net>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

BF = built-configfile.h
localshell.cxx: $(BF)
$(BF):
	echo "const char* CONFIGFILE = \"$(sysconfdir)/localshell.conf\";" >$@

bin_PROGRAMS = localshell
localshell_SOURCES  = localshell.cxx config.cxx common.cxx common.hh config.hh structures.hh
MAINTAINERCLEANFILES = `cat .cvsignore` $(BF)
BUILT_SOURCES = $(BF)
CLEANFILES = $(BF)
DISTCLEANFILES = $(BF)
nodist_localshell_SOURCES = built-configfile.h
