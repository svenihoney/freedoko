cmake_minimum_required(VERSION 2.6)

project(FreeDoko)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(ALUT freealut)
pkg_check_modules(GNET gnet-2.0)

set(DATA_INSTALL_DIR share/games/FreeDoko)
set(DOC_INSTALL_DIR share/doc/freedoko)

add_subdirectory(src) 
add_subdirectory(data) 
add_subdirectory(doc) 
