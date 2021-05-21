cmake_minimum_required(VERSION 3.9)

#  Without C++11, compile error
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cmake config
project(DataTable VERSION 1.0.0 DESCRIPTION "Data Tables for C++")
include(GNUInstallDirs)

## create shared library (.so file) which is the source code for the project
add_library(DataTable SHARED
	src/DataTable/DataTable.cc
	src/DataTable/StringUtils.cc
	src/DataTable/DateUtils.cc
)

## create list of includes (public headers)
## THese will be included in the programs using our program
set(PHEADERS 
	"include/DataTable/DataTable.hpp"
	"include/DataTable/StringUtils.hpp"
	"include/DataTable/DateUtils.hpp"
)

# some CMAKE config
# PHEADERS and PUBLIC_HEADER allow us to include the headers in our project, 
# i.e. they create the /usr/local/YALL/*.hpp files
set_target_properties(DataTable PROPERTIES 
	VERSION ${PROJECT_VERSION}
	SOVERSION 1
	PUBLIC_HEADER "${PHEADERS}")

## Create the pkg-config file
configure_file(datatable.pc.in datatable.pc @ONLY)

## not 100% sure here
## A good way to thinkg of this is that we're adding the second part (after the PRIVATE/PUBLIC/etc.)
## as compiler flags in the Makefile with the first value as the target (YALL) which are just Makefile
## targets.
target_include_directories(DataTable PRIVATE include)
target_include_directories(DataTable PRIVATE src)
## e.g. add the flag -Wno-psabi to the Makefile target YALL
target_compile_options(DataTable PRIVATE -Wno-psabi)

## Install the project shared library files and the PUBLIC HEADERS (files to be used by apps:).
install(TARGETS DataTable 
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DataTable
)

## builc the .pc pkg-config file and move it
install(FILES ${CMAKE_BINARY_DIR}/datatable.pc
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

## Need to copy these files to the include directories to be includeed by the public headers.
install(DIRECTORY "include/DataTable/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DataTable)

## need to find a better way
enable_testing()
add_test(DataSelectionTest "${PROJECT_SOURCE_DIR}/test/DataSelectionTest")
add_test(FileReadTest "${PROJECT_SOURCE_DIR}/test/FileReadTest")
