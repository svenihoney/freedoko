DEPTH = ..

include $(DEPTH)/Makefile.os

.PHONY: all
all : gpl.dos.string help.dos.string Altenburg.dos.string

include $(DEPTH)/Makefile.subdir
#include $(DEPTH)/Makefile.rules

ifeq ($(SHELLTYPE), sh)
%.dos.string : %.string
	@-echo "converting '$<' to '$@'" \
	  && cp $< $@ \
	  && ( flip -m $@ 2>/dev/null \
	      || unix2dos --quiet $@ 2>/dev/null)
endif

clean :
