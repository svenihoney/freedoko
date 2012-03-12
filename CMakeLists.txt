cmake_minimum_required(VERSION 2.6)

project(FreeDoko)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(GNET gnet-2.0)

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
