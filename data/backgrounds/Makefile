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
endif
