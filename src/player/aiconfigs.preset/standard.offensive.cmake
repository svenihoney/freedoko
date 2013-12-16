FILE(READ ${infile} f0)
STRING( REGEX REPLACE "standard.defensive" "standard_offensive" f1 "${f0}" )
STRING( REGEX REPLACE "(type *= *(8|9|10|11)*:)[^\\]*" "\\1 gametree for team" f1 "${f1}" )
STRING( REGEX REPLACE "(rating *= *[0-7]:)[^\\]*" "\\1 linear" f1 "${f1}" )
STRING( REGEX REPLACE "(trusting *=)[^\\]*" "\\1 true" f1 "${f1}" )
STRING( REGEX REPLACE "(aggressive *=)[^\\]*" "\\1 true" f1 "${f1}" )
STRING( REGEX REPLACE "(\nlimit queen *=)[^\\]*" "\\1  9" f1 "${f1}" )
STRING( REGEX REPLACE "(limit dolle *=)[^\\]*" "\\1 17" f1 "${f1}" )
STRING( REGEX REPLACE "(last fehlcreation *=)[^\\]*" "\\1 5" f1 "${f1}" )
STRING( REGEX REPLACE "(first trick for trump points optimization *=)[^\\]*" "\\1 5" f1 "${f1}" )
STRING( REGEX REPLACE "(announce limit *=)[^\\]*" "\\1 10" f1 "${f1}" )
STRING( REGEX REPLACE "(announce limit reply *=)[^\\]*" "\\1  9" f1 "${f1}" )
STRING( REGEX REPLACE "(take poverty *=)[^\\]*" "\\1 12" f1 "${f1}" )
STRING( REGEX REPLACE "(handvalue single solo =) [^\\]*" "\\1 16" f1 "${f1}" )
STRING( REGEX REPLACE "(handvalue double solo =) [^\\]*" "\\1 46" f1 "${f1}" )
STRING( REGEX REPLACE "(handvalue trible solo =) [^\\]*" "\\1 92" f1 "${f1}" )
STRING( REGEX REPLACE "(handvalue color solo =) [^\\]*" "\\1 26" f1 "${f1}" )
STRING( REGEX REPLACE "(handvalue meatless =) [^\\]*" "\\1 24" f1 "${f1}" )
STRING( REGEX REPLACE "( *grab trick =) (true|false)" "\\1 true" f1 "${f1}" )
FILE(WRITE ${outfile} "${f1}")