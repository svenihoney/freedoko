FILE(READ ${infile} f0)
STRING( REGEX REPLACE "\n" "\r\n" f1 "${f0}" )
FILE(WRITE ${outfile} "${f1}")
