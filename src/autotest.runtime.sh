#!/bin/sh

# compile FreeDoko and run the runtime test party:
# *  standard rules
# *  limited rounds
# *  profi players


# create program in release version
	  
rm ./FreeDoko; make "CXXFLAGS=-Wall -Werror -Wno-parentheses -pipe -O0" "USE_UI_GTKMM=false" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.runtime/src"
#rm ./FreeDoko; make "CXXFLAGS=-Wall -Werror -Wno-parentheses -pipe -O3 -DNO_UPDATE_ON_DEMAND" "USE_UI_GTKMM=false" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.runtime/src"
#rm ./FreeDoko; make "CXXFLAGS=-Wall -Werror -pipe -O0" "USE_UI_GTKMM=false" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.runtime/src"

nice -20 ./FreeDoko ./testparty.runtime -F 123 --ui=none "$@"
