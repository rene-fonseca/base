cmake_minimum_required (VERSION 3.1)

project (testsuite)

file( GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp )
# list(REMOVE_ITEM TEST_SOURCES "")

foreach( sourcefile ${TEST_SOURCES} )
  string( REPLACE ".cpp" "" name ${sourcefile} )
  add_executable( ${name} ${sourcefile} )
  # add_dependencies( ${name} base )
  # target_link_libraries( ${name} "${CMAKE_CURRENT_BINARY_DIR}/../../DEBUG/base.lib" )
  add_dependencies( ${name} base_STATIC )
if (MSVC)
  target_link_libraries( ${name} "${CMAKE_CURRENT_BINARY_DIR}/../../DEBUG/base_static.lib" )
  target_link_libraries( ${name} Ws2_32 Netapi32 Winmm) # TAG: get from global variable
else ()
  target_link_libraries( ${name} base )
endif ()
endforeach()
