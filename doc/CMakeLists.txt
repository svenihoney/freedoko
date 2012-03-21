include(${CMAKE_SOURCE_DIR}/cmake/UseLATEX.cmake)

add_subdirectory(de)
add_subdirectory(en)
  
install(DIRECTORY Implementation
        DESTINATION ${DOC_INSTALL_DIR})
install(FILES Bugs GPL Roadmap
        DESTINATION ${DOC_INSTALL_DIR})
