FILE(READ ${infile} f0)
STRING( REGEX REPLACE "^# FreeDoko[^\n]*\n" "" f1 "${f0}" )
STRING( REGEX REPLACE "\r?\n" "\\\\n\\\\\n" f3 "${f1}" )
SET( f4 "// This is automatically generated code -- do not edit!
// aiconfig: standard defensive
char const standard_defensive[] = \"\\\n${f3}\";\n" )
FILE(WRITE ${outfile} "${f4}")
