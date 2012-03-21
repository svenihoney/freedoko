generate_with_cmake(${CMAKE_CURRENT_SOURCE_DIR}/gpl.string gpl.dos.string to_dos.cmake) 
generate_with_cmake(${CMAKE_CURRENT_SOURCE_DIR}/help.string help.dos.string to_dos.cmake) 
generate_with_cmake(${CMAKE_CURRENT_SOURCE_DIR}/Altenburg.string Altenburg.dos.string to_dos.cmake) 

ADD_CUSTOM_TARGET(texts DEPENDS gpl.dos.string help.dos.string Altenburg.dos.string)
