include(CMakeChecks.txt)

macro(toglobalsources LOCAL_SOURCES)
  foreach(SRC ${ARGV})
    list(APPEND GLOBAL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${SRC})
  endforeach(SRC)
  set(FREEDOKO_SOURCES ${FREEDOKO_SOURCES} ${GLOBAL_SOURCES} PARENT_SCOPE)
endmacro(toglobalsources)

include_directories(SYSTEM
  ${GTKMM_INCLUDE_DIRS} 
  ${GNET_INCLUDE_DIRS}
  ${ALUT_INCLUDE_DIRS} 
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/player
)

if(GTKMM_FOUND)
  add_definitions( -DUSE_UI_GTKMM )
endif(GTKMM_FOUND)

if(GNET_FOUND)
  add_definitions( -DUSE_NETWORK )
  add_subdirectory(network)
endif(GNET_FOUND)

if(ALUT_FOUND)
  add_definitions( -DUSE_SOUND -DUSE_SOUND_ALUT )
  add_subdirectory(sound)
endif(ALUT_FOUND)

if(Boost_FOUND)
  add_definitions( -DBOOST )
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

if(Threads_FOUND)
  add_definitions( -DUSE_THREADS )
endif(Threads_FOUND)

if(PUBLIC_DATA_DIRECTORY)
  add_definitions( -DPUBLIC_DATA_DIRECTORY_VALUE=${PUBLIC_DATA_DIRECTORY} )
else(PUBLIC_DATA_DIRECTORY)
  add_definitions( -DPUBLIC_DATA_DIRECTORY_VALUE="${CMAKE_SOURCE_DIR}/data"
    -DMANUAL_DIRECTORY_VALUE="${CMAKE_SOURCE_DIR}/doc/manual"
    -DVERSION_DESCRIPTION="self compiled"
    )
endif(PUBLIC_DATA_DIRECTORY)

if(CMAKE_SYSTEM_NAME MATCHES Linux.*)
  add_definitions( -DLINUX )
endif(CMAKE_SYSTEM_NAME MATCHES Linux.*)

link_directories(
	    ${GTKMM_LIBRARY_DIRS} ${GNET_LIBRARY_DIRS} ${ALUT_LIBRARY_DIRS} )

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror -Wno-unused-value -Wno-parentheses")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused-value -Wno-parentheses")
endif()

ADD_CUSTOM_COMMAND(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/logo.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/logo.free.png ${CMAKE_CURRENT_BINARY_DIR}/logo.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/icon.png ${CMAKE_CURRENT_BINARY_DIR}/icon.png
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/logo.free.png
)
ADD_CUSTOM_TARGET(copy_logos DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/logo.png)

add_subdirectory(card)
add_subdirectory(game)
add_subdirectory(class)
add_subdirectory(misc)
add_subdirectory(os)
add_subdirectory(party)
add_subdirectory(player)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(FreeDoko
  basistypes.cpp  
  constants.cpp 
  debug.cpp 
  freedoko.cpp 
  runtime.cpp
  utils.cpp

  ${FREEDOKO_SOURCES}
)

add_dependencies(FreeDoko copy_logos aiconfigs)

target_link_libraries(FreeDoko 
  ${GTKMM_LIBRARIES} ${GNET_LIBRARIES} ${ALUT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS FreeDoko
        RUNTIME DESTINATION games)
install(FILES icon.png
        DESTINATION ${DATA_INSTALL_DIR})
install(FILES logo.free.png
        DESTINATION ${DATA_INSTALL_DIR}
		RENAME logo.png )

