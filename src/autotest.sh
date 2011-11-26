#!/bin/sh

# compile FreeDoko and run the test party:
# *  mutating rules
# *  no end of the party
# *  four different ai players (standard, profi, offensive, profi unfair)

# non-release-version

if [ $# -gt 0 ]; then
  if [ "$1" = "text" ]; then
    shift
    make \
    USE_UI_GTKMM=false \
    USE_SOUND=false \
    USE_NETWORK=false \
    USE_THREADS=true \
    CXXFLAGS="-Wall -Werror -Wno-parentheses -pipe -O0 -ggdb" \
    FREEDOKO_WORKING_DIRECTORY="/home/tmp/${LOGNAME}/FreeDoko.autotest" \
    && ./FreeDoko testparty -F 251 "$@"
    exit
  fi
fi


# create program in release version
make --always-make release_bin && cd ../data
#rm ./FreeDoko; make "CXXFLAGS=-Wall -Werror -Wno-parentheses -pipe -O0 -ggdb" "USE_UI_GTKMM=false" "FREEDOKO_WORKING_DIRECTORY=/tmp/${LOGNAME}/FreeDoko.autotest/src"

# start from ../data, so that all data files are found
../src/FreeDoko ../src/testparty -F $(( 1+2+8+16+32+64+128 )) "$@"
#../src/FreeDoko ../src/testparty.random -F $(( 1+2+8+16+32+64+128 )) "$@"
# --auto-bug-reports 
