DEPTH = ../..

include $(DEPTH)/Makefile.os
include $(DEPTH)/Makefile.install.directories

.PHONY: all
all :

.PHONY: release
release :
ifeq ($(SHELLTYPE), sh)
	mkdir -p $(RELEASE_TMP)/FreeDoko_$(VERSION)/cardsets/
	for d in xskat; do \
	  if [ -d $$d ]; then \
	    tar -c --exclude=.svn $$d \
	      | tar -xC $(RELEASE_TMP)/FreeDoko_$(VERSION)/cardsets/; \
	  fi \
	done
	@# In order to add all cardsets to the release
	@# uncomment the following line, add a trailing backslash
	@# and remove the line after
	@#for d in InnoCard freedoko-pingucards gnome-games kdecarddecks openclipart pysol; do
	for d in InnoCard freedoko-pingucards; do \
	  if [ -d $(CARDSETS)/$$d ]; then \
	    tar -c --exclude=.svn -C $(CARDSETS) $$d \
	      | tar -xC $(RELEASE_TMP)/FreeDoko_$(VERSION)/cardsets/; \
	  fi \
	done
	chmod -R u+w $(RELEASE_TMP)/FreeDoko_$(VERSION)/cardsets/
endif
