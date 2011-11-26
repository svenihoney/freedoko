DEPTH = ../..
SUBDIR = ./player/aiconfigs.preset

include $(DEPTH)/Makefile.os

.PHONY: all
all : aiconfigs


.PHONY: aiconfigs
aiconfigs: standard.defensive standard.offensive profi profi.aggressive profi.unfair

novice : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif

standard.defensive : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif

standard.offensive : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif

profi : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif

profi.aggressive : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif

profi.unfair : standard.defensive.saved
ifeq ($(SHELLTYPE), sh)
	sh ./create_standard_aiconfigs
endif


include $(DEPTH)/Makefile.subdir
