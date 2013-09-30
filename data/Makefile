DEPTH = ..

include $(DEPTH)/Makefile.version
include $(DEPTH)/Makefile.os
include $(DEPTH)/Makefile.install.directories

.PHONY: all
all :

ifeq ($(OS), Linux)
.PHONY: zips
zips :
	for d in translations rules ai; do \
	  zip -qr $${d}.zip $$d; \
	done
endif

ifeq ($(SHELLTYPE), sh)
.PHONY: release
release :
	mkdir -p $(RELEASE_TMP)/FreeDoko_$(VERSION)/
	for d in translations rules ai; do \
	  tar -c --exclude=.svn $$d \
	    | tar -xC $(RELEASE_TMP)/FreeDoko_$(VERSION)/; \
	done
	$(MAKE) -C ../src logo.png
	$(MAKE) -C ../src icon.png
	cp ../src/logo.png ../src/icon.png $(RELEASE_TMP)/FreeDoko_$(VERSION)/
	$(MAKE) -C cardsets release
	$(MAKE) -C iconsets release
	$(MAKE) -C backgrounds release
	$(MAKE) -C sounds release
endif

ifeq ($(SHELLTYPE), sh)
.PHONY: clean
clean :
	$(MAKE) -C setup/Windows clean
endif
