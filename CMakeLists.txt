cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0011 OLD)

project(FreeDoko)


#pkg_check_modules(GTKMM gtkmm-2.4)
find_package(GTK2 2.4 REQUIRED gtkmm)
add_definitions(-DUSE_UI_GTKMM)
if(NOT CMAKE_CROSSCOMPILING)
  find_package(PkgConfig)
  pkg_check_modules(GTKMM gtkmm-2.4)
  pkg_check_modules(GNET gnet-2.0)
endif(NOT CMAKE_CROSSCOMPILING)

if(USE_SOUND)
  pkg_check_modules(ALUT freealut)
endif(USE_SOUND)

if(USE_BOOST)
  find_package(Boost COMPONENTS filesystem system)
endif(USE_BOOST)

option(USE_THREADS "Use thread library (experimental)")
if(USE_THREADS)
  find_package(Threads)
endif(USE_THREADS)

set(DATA_INSTALL_DIR share/games/FreeDoko)
set(DOC_INSTALL_DIR share/doc/freedoko)

add_subdirectory(src) 
add_subdirectory(data) 
add_subdirectory(doc) 
