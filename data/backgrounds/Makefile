DEPTH = ../..

include $(DEPTH)/Makefile.os
include $(DEPTH)/Makefile.install.directories

.PHONY: all
all :

.PHONY: release
release :
ifeq ($(SHELLTYPE), sh)
	mkdir -p $(RELEASE_TMP)/FreeDoko_$(VERSION)/backgrounds/
	cp table.png $(RELEASE_TMP)/FreeDoko_${VERSION}/backgrounds/
	for d in christmas wood; do \
	  tar -c --exclude=.svn $$d \
	    | tar -xC $(RELEASE_TMP)/FreeDoko_$(VERSION)/backgrounds/; \
	done
endif
