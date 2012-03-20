FILE(READ ${infile} f0)
STRING( REGEX REPLACE "profi" "profi_aggressive" f1 "${f0}" )
STRING( REGEX REPLACE "(aggressive *=)[^\\]*" "\\1 true" f1 "${f1}" )
FILE(WRITE ${outfile} "${f1}")
