DEPTH = .
SUBDIR = .

# for Windows under Linux:
# export OSTYPE=Linux_to_Windows

include $(DEPTH)/Makefile.os

-include Makefile.objects

DIRECTORIES = party game player card misc ui os class utils text
OBJECTS_ALL = $(OBJECTS) \
		$(OBJECTS_PARTY) $(OBJECTS_GAME) $(OBJECTS_PLAYER) \
		$(OBJECTS_CARD) $(OBJECTS_MISC) \
		$(OBJECTS_UI) $(OBJECTS_OS) \
		$(OBJECTS_CLASS) $(OBJECTS_UTILS)
ifeq ($(OPERATING_SYSTEM), Windows)
OBJECTS_ALL += windres.o
endif
INCLUDE_ALL = $(INCLUDE)
LIBS_ALL = $(LIBS) -lm
ifeq ($(USE_BOOST), true)
	LIBS_ALL += $(LIBS_BOOST)
endif
ifeq ($(USE_UI_GTKMM), true)
	OBJECTS_ALL += $(OBJECTS_GTKMM)
	INCLUDE_ALL += $(INCLUDE_GTKMM)
	LIBS_ALL += $(LIBS_GTKMM)
endif
ifeq ($(USE_SOUND), true)
	DIRECTORIES += sound
	OBJECTS_ALL += $(OBJECTS_SOUND)
ifeq ($(USE_SOUND_ALUT), true)
	INCLUDE_ALL += $(INCLUDE_SOUND_ALUT)
	LIBS_ALL += $(LIBS_SOUND_ALUT)
endif
ifeq ($(USE_SOUND_PLAYSOUND), true)
	LIBS_ALL += $(LIBS_SOUND_PLAYSOUND)
endif
endif
ifeq ($(USE_NETWORK), true)
	DIRECTORIES += network
	OBJECTS_ALL += $(OBJECTS_NETWORK)
	INCLUDE_ALL += $(INCLUDE_NETWORK)
	LIBS_ALL += $(LIBS_NETWORK)
endif


# check the environment and compile the program
.PHONY: all
all : Makefile.local logo.png
	$(MAKE) check
	$(MAKE) $(PROGRAM)

# create the file 'Makefile.local' from the template
Makefile.local:
ifeq ($(SHELLTYPE), sh)
	test -f $@ || cp Makefile.local.template $@
endif
ifeq ($(SHELLTYPE), COMMAND.COM)
	if not exist $@ copy Makefile.local.template $@
endif

# create the logo from the free one
logo.png:
ifeq ($(SHELLTYPE), sh)
	test -f $@ || cp logo.free.png $@
endif
ifeq ($(SHELLTYPE), COMMAND.COM)
	if not exist $@ copy logo.free.png $@
endif


# print the targets of this makefile
.PHONY: help
help :
	@echo "make targets"
	@echo "  all               check the environment and compile the program"
	@echo "  help              print the targests of this makefile"
	@echo "  check             check the os and the libraries"
	@echo "  os_check          check, whether the ostype was detected correctly"
	@echo "  libraries_check   "check, whether the needed libraries are installed
	@echo "  watch             automatic compile FreeDoko if a *.cpp-file has changed"
	@echo "  seed.list         create a file with information of the hands for different seeds"
	@echo "  show_settings     show all settings (environment, modules, flags)"
	@echo "  show_environment  show the environment settings"
	@echo "  show_modules      show the modules"
	@echo "  show_flags        show the flags used for compiling"
	@echo "  FreeDoko          compile FreeDoko"
	@echo "  release_bin       compile FreeDoko for a release"
	@echo "  one_compile       create one file and compile the program (does not work)"
	@echo "  fast              compile FreeDoko with three processes at once (like '-j 3')"
	@echo "  strip             strip the program from all symbols"
	@echo "  objects           compile all cpp files in this directoriy"
	@echo "  check_references  checks the references"
	@echo "  doc               create api overview"
	@echo "  tags              create a tags file"
	@echo "  update_aiconfigs  updates the aiconfigs (from the hardcoded)"
	@$(MAKE) -s rules_help

# check the os and the libraries
.PHONY: check
check : os_check libraries_check

abspath:
	echo $(abspath C:\\tmp)
	echo $(abspath C:\tmp)
	echo $(abspath C:/tmp)
	echo $(abspath .)

# check, whether the ostype was detected correctly
.PHONY: os_check
os_check :
ifeq ($(OPERATING_SYSTEM), Windows)
 ifneq ($(SHELLTYPE), sh)
	@REM \
	  ; (echo -e \\n\
	  	      "***** Error *****\\n"\
	              "If you see these lines, the operating system was not detected correctly.\\n"\
	              "Detected operating system: $(OPERATING_SYSTEM)\\n"\
	              "Try an 'export OSTYPE' before compiling.\\n"\
	      && false)
endif
else
	@# echo "***** Error ***** If you see this line, the operating system was not detected correctly. Detected operating system: $(OPERATING_SYSTEM)"
endif

# check, whether the needed libraries are installed
# till now only supports 'pkg-config', t.i. unix systems
.PHONY: libraries_check
libraries_check :
ifeq ($(SHELLTYPE), sh)
  ifeq ($(USE_UI_GTKMM), true)
	@pkg-config --exists gtkmm-2.4 \
	  || (echo "gtkmm 2.4 (user interface) is not found."; \
	      export problem=true)
endif
  ifeq ($(USE_NETWORK), true)
	@pkg-config --exists glib-2.0 \
	  || echo "glib 2.0 (needed for gnet) is not found."
	@pkg-config --exists gnet-2.0 \
	  || echo "gnet 2.0 (network library) is not found."
endif
endif
ifeq ($(SHELLTYPE), COMMAND.COM)
#       ToDo
endif

# automatic compile FreeDoko if a *.cpp-file has changed
# uses program 'auto_execute' (written by Diether Knof, dknof@gmx.de)
.PHONY: watch
watch :
	# Does not work, too many files?
	# auto_execute -e 'nice make -k' `find . -name "*.cpp" -o -name "*.h"`
	rm -f commands.sh
	echo '#!/bin/bash' >> commands.sh
	echo 'if [ -e ./FreeDoko ]; then' >> commands.sh
	echo '  for f in $$(find . -name "*.cpp"); do' >> commands.sh
	echo '    if [ $$f -nt ./FreeDoko ]; then rm ./FreeDoko; break; fi' >> commands.sh
	echo '  done' >> commands.sh
	echo 'fi' >> commands.sh
	echo 'if [ $$(grep -c processor /proc/cpuinfo) -gt 1 ]; then' >> commands.sh
	echo '  nice -n 5 make -s -k --jobs=$$(expr 1 + $$(grep -c processor /proc/cpuinfo) ) --load-average=$$(expr 1 + $$(grep -c processor /proc/cpuinfo) ) && sleep 1' >> commands.sh
	echo 'else' >> commands.sh
	echo '  nice -n 5 make -s -k && sleep 1' >> commands.sh
	echo 'fi' >> commands.sh
	auto_execute -e 'bash ./commands.sh' `find . -name "*.cpp"` Makefile.local
	rm -f commands.sh

# create a file with the gametype for the different seeds
# The program must be prepared (in 'constants.cpp')
seed.info : $(PROGRAM)
	./$(PROGRAM) --seed-info --seed=3 --no-automatic-savings ./party.with-nines  | gzip -9 - > seed.info.with-nines.csv.gz
	./$(PROGRAM) --seed-info --seed=3 --no-automatic-savings ./party.without-nines | gzip -9 - > seed.info.without-nines.csv.gz

# show the settings
.PHONY: show_settings
show_settings : show_environment show_modules show_flags

# show the environment
.PHONY: show_environment
show_environment :
	@echo 'Environment'
	@echo '  OPERATING_SYSTEM           : $(OPERATING_SYSTEM)'
	@echo '  OPERATING_SYSTEM_NAME      : $(OPERATING_SYSTEM_NAME)'
	@echo '  SHELLTYPE                  : $(SHELLTYPE)'
ifneq ($(MINGW_DIR),)
	@echo '  MINGW_DIR                  : $(MINGW_DIR)'
endif
	@echo '  PROGRAM                   : $(PROGRAM)'
	@echo '  FREEDOKO_WORKING_DIRECTORY : $(FREEDOKO_WORKING_DIRECTORY)'

# show the modules used
.PHONY: show_modules
show_modules :
	@echo 'Modules'
	@echo '  USE_THREADS  : $(USE_THREADS)'
	@echo '  USE_BOOST    : $(USE_BOOST)'
	@echo '  USE_UI_TEXT  : $(USE_UI_TEXT)'
	@echo '  USE_UI_GTKMM : $(USE_UI_GTKMM)'
	@echo '  USE_SOUND    : $(USE_SOUND)'
	@echo '    USE_SOUND_ALUT      : $(USE_SOUND_ALUT)'
	@echo '    USE_SOUND_APLAY     : $(USE_SOUND_APLAY)'
	@echo '    USE_SOUND_PLAYSOUND : $(USE_SOUND_PLAYSOUND)'
	@echo '  USE_NETWORK  : $(USE_NETWORK)'
	@echo '    USE_NETWORK_DOKOLOUNGE  : $(USE_NETWORK_DOKOLOUNGE)'

# show the flags used for compiling
.PHONY: show_flags
show_flags :
	@echo 'Flags'
	@echo '  CXX      : $(CXX)'
	@echo '  CPPFLAGS : $(CPPFLAGS)'
	@echo '  CXXFLAGS : $(CXXFLAGS)'
	@echo '  INCLUDE  : $(INCLUDE_ALL)'
	@echo '  LIBS     : $(LIBS_ALL)'

# compile FreeDoko
$(PROGRAM) : show_settings objects $(DIRECTORIES)
	@echo "compiling $(PROGRAM)..."
# Gentoo users do want to see the real compile line.
# So remove the next five lines (the echo lines)
# and remove the '@' in the line after.
	@echo '$(CXX) $(CXXFLAGS)'
	@echo '        $$(OBJECTS)'
	@echo '        $(INCLUDE_ALL)'
	@echo '        $(LIBS_ALL)'
	@echo '        -o $@'
	@$(CXX) $(CXXFLAGS) \
	  $(OBJECTS_ALL:%.o=$(FREEDOKO_WORKING_DIRECTORY)/%.o) \
	  $(INCLUDE_ALL) \
	  $(LIBS_ALL) \
	  -o $@
	@echo "$(PROGRAM) compiled successfully"

ifeq ($(OPERATING_SYSTEM), Linux)
.PHONY: FreeDoko.exe
FreeDoko.exe :
	$(MAKE) OSTYPE=Linux_to_Windows $@
endif

# compile FreeDoko for a release
.PHONY: release_bin
release_bin : Makefile.local logo.png
ifeq ($(OPERATING_SYSTEM), Windows)
	$(MAKE) \
	  "USE_THREADS=false" \
	  "USE_UI_TEXT=false" \
	  "USE_UI_GTKMM=true" \
	  "USE_NETWORK=true" \
	  "USE_NETWORK_DOKOLOUNGE=false" \
	  "USE_BOOST=false" \
	  "USE_SOUND_ALUT=false" \
	  "USE_SOUND_COMMAND=false" \
	  "USE_SOUND_PLAYSOUND=true" \
	  "CXXFLAGS=-march=i686 -pipe -Os" \
	  "CPPFLAGS=-DRELEASE -DVERSION_DESCRIPTION='\"$(OPERATING_SYSTEM_NAME) binary\"'" \
	  "LIBS=-mwindows" \
	  $(PROGRAM)
	$(MAKE) strip
else
	$(MAKE) \
	  "USE_THREADS=false" \
	  "USE_UI_TEXT=false" \
	  "USE_UI_GTKMM=true" \
	  "USE_NETWORK=true" \
	  "USE_NETWORK_DOKOLOUNGE=false" \
	  "USE_BOOST=false" \
	  "USE_SOUND_ALUT=true" \
	  "USE_SOUND_COMMAND=false" \
	  "USE_SOUND_PLAYSOUND=false" \
	  "CXXFLAGS=-mtune=generic -Wall -Werror -Wno-parentheses -pipe -Os" \
	  "CPPFLAGS=-DRELEASE -DVERSION_DESCRIPTION='\"$(OPERATING_SYSTEM_NAME) binary\"'" \
	  $(PROGRAM)
	$(MAKE) strip
endif

# test code, does not work
# Create one file and compile from it the program.
# Use this in order to test package creating
# (this uses less time than compiling all files one for one)
.PHONY: one_compile
one_compile : show_settings
	@echo "compiling $(PROGRAM)..."
	# Gentoo users do want to see the real compile line.
	# So remove the following echo lines
	# and remove the '@' in the lines after.
	@echo '$(CXX) $(CPPFLAGS)'
	@echo '        $$(OBJECTS).cpp'
	@echo '        $(INCLUDE_ALL)'
	@echo '        -E -o $(PROGRAM).ii'
	@$(CXX) $(CPPFLAGS) \
	$(patsubst %.o,%.cpp,$(OBJECTS_ALL)) \
	$(INCLUDE_ALL) \
	-E > FreeDoko.ii
	@echo '$(CXX) $(CXXFLAGS)'
	@echo '        -c $(PROGRAM).ii'
	# errors
	@$(CXX) $(CXXFLAGS) -c -x c++-cpp-output $(PROGRAM).ii
	# $(LIBS_ALL)
	#-o $(PROGRAM)
	@echo '$(CXX) $(LIBS_ALL)'
	@echo '       $(PROGRAM).o'
	@echo '        -o $(PROGRAM)'
	@$(CXX) $(LIBS_ALL) $(PROGRAM).o -o $(PROGRAM)
	@echo "$(PROGRAM) compiled successfully"

# compile FreeDoko with three processes at once (like '-j 3')
ifeq ($(SHELLTYPE), sh)
.PHONY: fast
fast :
	nice $(MAKE) -k -C ui &
	( nice $(MAKE) -k -C party; \
	nice $(MAKE) -k -C game; \
	nice $(MAKE) -k -C card ) &
	( nice $(MAKE) -k -C player; \
	nice $(MAKE) -k objects; \
	nice $(MAKE) -k -C misc; \
	nice $(MAKE) -k -C os; \
	nice $(MAKE) -k -C class; \
	nice $(MAKE) -k -C utils )
endif

# strip the program from all symbols
.PHONY: strip
strip :
	$(STRIP) $(PROGRAM)

# checks the references
ifeq ($(SHELLTYPE), sh)
  .PHONY : check_references
check_references :
	$(RM) $(PROGRAM)
	$(MAKE) "CXXFLAGS=-Wall -Werror -Wno-parentheses -pipe -O0 -ggdb" "USE_UI_GTKMM=false" "USE_UI_TEXT=false" "USE_SOUND=false" "USE_BOOST=true" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.check_reference/src"
	#	$(MAKE) "CXXFLAGS=-Wall -Werror -Wno-parentheses -pipe -Os" "USE_UI_GTKMM=false" "USE_UI_TEXT=false" "USE_SOUND=false" "USE_BOOST=true" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.check_reference/src"
	cd ../data
	../src/$(PROGRAM) -r ../References --ui=none
	@-grep -h -v OK ../References/References.report.csv | grep "^[^;]*[0-9]\+\(_[0-9]\+\)\?.Reference.FreeDoko" | sort -n
	@tail -n 5 ../References/References.report.csv | sed "s/\;/\t/g";
	@echo -e "\nchecks entries "`grep "^[ \t]*check" ../References/*.Reference.FreeDoko ../References/**/*.Reference.FreeDoko | wc -l`
	@echo -e "failed: "; grep "^[^;]*[0-9]\+_\?[0-9]*.Reference.FreeDoko;" ../References/References.report.csv | grep -v OK | awk -F \; '{printf("  %s\n", $$1)}' | sort -n; echo
	@rm -rf /tmp/FreeDoko.References.failed
	@mkdir /tmp/FreeDoko.References.failed
	@for f in References.report.csv `grep "^[^;]*[0-9]\+\(_[0-9]\+\)\?.Reference.FreeDoko;" ../References/References.report.csv | grep -v OK | awk -F \; '{printf(" %s\\n", $$1)}'`; do cp ../References/$$f /tmp/FreeDoko.References.failed/; done
	@$(MAKE) >/dev/null
endif

# create api overview
# uses doxygen
# bug: does not create all documentation, since doxygen has problem with two different files with the same name
ifeq ($(OPERATING_SYSTEM), Linux)
  .PHONY : doc
doc :
	doxygen
endif

# create a tags file
ifeq ($(OPERATING_SYSTEM), Linux)
  .PHONY : tags
tags :
	ctags -R
endif

ifeq ($(OPERATING_SYSTEM), Linux)
  .PHONY : update_aiconfigs
update_aiconfigs :
	make $(PROGRAM)
	./$(PROGRAM) --save-standard-aiconfig
	cp $(HOME)/.FreeDoko/ai/standard.defensive.saved player/aiconfigs.preset/
endif

include $(DEPTH)/Makefile.subdir
include $(DEPTH)/Makefile.rules

ifeq ($(OPERATING_SYSTEM), Windows)
  # the program is dependent on the windres object file
  $(PROGRAM) : $(TARGET_DIR_LOCAL)windres.o

# create the windres object file
$(TARGET_DIR_LOCAL)windres.o : windres.rc
	windres $< $@
endif
