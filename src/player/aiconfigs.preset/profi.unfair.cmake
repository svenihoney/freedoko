FILE(READ ${infile} f0)
STRING( REGEX REPLACE "profi" "profi_unfair" f1 "${f0}" )
STRING( REGEX REPLACE "(hands known *=)[^\\]*" "\\1 true" f1 "${f1}" )
STRING( REGEX REPLACE "linear" "maximum" f1 "${f1}" )
FILE(WRITE ${outfile} "${f1}")