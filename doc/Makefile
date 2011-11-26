DEPTH = ..

DIRECTORIES = de en

include $(DEPTH)/Makefile.version
include $(DEPTH)/Makefile.os
include $(DEPTH)/Makefile.install.directories

.PHONY: all
all: $(DIRECTORIES)

.PHONY : $(DIRECTORIES)
$(DIRECTORIES) :
	$(MAKE) -C $@


ifeq ($(SHELLTYPE), sh)
.PHONY: release
release :
	mkdir -p $(RELEASE_TMP)/FreeDoko_$(VERSION)/doc
	$(MAKE) -C en release 
	$(MAKE) -C de release 
	$(MAKE) -C manual release 
	tar -c --exclude=.svn Implementation \
	   | tar xC $(RELEASE_TMP)/FreeDoko_$(VERSION)/doc/
	cp Bugs Roadmap GPL hpux.required_libs-ia64.txt freedoko.sgml \
	  $(RELEASE_TMP)/FreeDoko_$(VERSION)/doc/
endif

ifeq ($(SHELLTYPE), sh)
.PHONY: clean
clean :
	-find . -name "*~" -exec rm \{\} \;
	-rm -f a2out.ps
	-rm -rf doxygen
	for d in *; do\
	  if ( test -d "$$d" && test -f "$$d"/Makefile ); then \
	    $(MAKE) -C "$$d" clean; \
	  fi; \
	done
endif

ifeq ($(SHELLTYPE), sh)
.PHONY: usb_copy
usb_copy :
	$(MAKE) release
	for f in $(DIRECTORIES); do \
	  $(MAKE) -C $$f \
	  && cp $$f/*.txt $$f/*.pdf ../release/FreeDoko_$(VERSION)/doc/$$f/; \
	done
	-mount /media/usbstick
	tar czf /media/usbstick/FreeDoko.doc.tgz -C ../release/FreeDoko_$(VERSION)/ doc
	rm -rf ../release/FreeDoko_$(VERSION)
endif
