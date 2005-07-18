# Copyright (C) 2005 Robin Hugh Johnson <robbat2@orbis-terrarum.net>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

SUBDIRS = src doc
MAINTAINERCLEANFILES = `cat .cvsignore`

dist_docs = INSTALL AUTHORS COPYING ChangeLog THANKS
# built from README.in
built_docs = README
docs = $(dist_docs) $(built_docs)
EXTRA_DIST = $(docs)
docsdir = $(datadir)/doc/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/
docs_DATA = $(docs)

#noinst_DATA = $(built_DOCS)
EXTRA_DIST = configure reconf $(dist_DOCS)
