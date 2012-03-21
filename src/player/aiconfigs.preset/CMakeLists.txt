MACRO (GENERATE_CONFIG infile outfile)
  generate_with_cmake(${infile} ${outfile} ${outfile}.cmake)
ENDMACRO (GENERATE_CONFIG infile outfile)

generate_config(${CMAKE_CURRENT_SOURCE_DIR}/standard.defensive.saved standard.defensive) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/standard.defensive novice) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/standard.defensive standard.offensive) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/standard.defensive profi) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/profi profi.aggressive) 
generate_config(${CMAKE_CURRENT_BINARY_DIR}/profi profi.unfair) 

ADD_CUSTOM_TARGET(aiconfigs DEPENDS novice standard.offensive 
  profi profi.aggressive profi.unfair )
