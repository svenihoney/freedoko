DEPTH = ..
SUBDIR = ./player

include $(DEPTH)/Makefile.os

DIRECTORIES = rating human ai ai.dummy ai.random network aiconfigs.preset

include Makefile.objects

.PHONY: all
all : directories objects 


include $(DEPTH)/Makefile.rules
include $(DEPTH)/Makefile.subdir
