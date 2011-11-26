DEPTH = ..
SUBDIR = ./network

include $(DEPTH)/Makefile.os

DIRECTORIES = actor gnet FreeDoko

include Makefile.objects

INCLUDE += $(INCLUDE_NETWORK)
LIBS += $(LIBS_NETWORK)

.PHONY: all
all : objects directories


include $(DEPTH)/Makefile.rules
include $(DEPTH)/Makefile.subdir
