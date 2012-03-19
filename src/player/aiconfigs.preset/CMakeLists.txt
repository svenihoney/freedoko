MACRO (GENERATE_CONFIG infile outfile)
ADD_CUSTOM_COMMAND( 
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${outfile}
  COMMAND ${CMAKE_COMMAND}
  ARGS
    -Dinfile:FILEPATH=${infile}
    -Doutfile:FILEPATH=${CMAKE_CURRENT_BINARY_DIR}/${outfile}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/${outfile}.cmake
  MAIN_DEPENDENCY ${infile}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${outfile}.cmake
)
ENDMACRO (GENERATE_CONFIG)

generate_config(${CMAKE_CURRENT_SOURCE_DIR}/standard.defensive.saved standard.defensive) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/standard.defensive novice) 

#ADD_CUSTOM_COMMAND( 
#  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/novice
#  COMMAND sed -e 's.^\(// aiconfig:\) standard defensive$.\1 novice.' 
#    -e 's.^\(char const\) standard_defensive\(\[\] = \"\).\1 novice\2.' 
#    -e 's.^\(type\s*=\s*[0-9]\{1,2\}:\) [a-z -]*.\1 monte carlo - jab or serve.' 
#    -e 's.^\(future limit\s*=\s*[0-9]\{1,2\}:\) [0-9]*.\1 1200.' 
#    -e 's.^\(rating\s*=\s*[0-9]\{1,2\}:\) [a-z ]*.\1 average.' 
#    -e 's.^\(trusting =\) \(true\|false\).\1 true.'
#    -e 's.^\(remember tricks =\) [0-9]\{1,2\}.\1 6.'
#    -e 's.^\(last fehlcreation =\) [0-9]\{1,2\}.\1 6.'
#    -e 's.^\(first trick for trump points optimization =\) [0-9]\+.\1 5.'
#    -e 's.^\(announce limit reply =\) [0-9]\+.\1 12.'
#    -e 's.^\(take poverty =\) [0-9]\+.\1 12.'
#    < ${CMAKE_CURRENT_BINARY_DIR}/standard.defensive  > ${CMAKE_CURRENT_BINARY_DIR}/novice
#  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/standard.defensive
#)

ADD_CUSTOM_TARGET(aiconfigs DEPENDS novice)
