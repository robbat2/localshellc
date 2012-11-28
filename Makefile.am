# Copyright (C) 2005 Robin Hugh Johnson <robbat2 AT orbis-terrarum.net>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBDIRS = src doc
MAINTAINERCLEANFILES = `cat .cvsignore .gitignore`

dist_docs = INSTALL AUTHORS COPYING README ChangeLog
docs = $(dist_docs) $(built_docs)
EXTRA_DIST = $(docs) configure reconf
docsdir = $(datadir)/doc/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/
docs_DATA = $(docs)

check-git-repository:
	git diff --quiet || { echo 'STOP, you have uncommitted changes in the working directory' ; false ; }
	git diff --cached --quiet || { echo 'STOP, you have uncommitted changes in the index' ; false ; }

ChangeLog: .git/index
	    git log >$@

dist: check-git-repository $(EXTRA_DIST)
