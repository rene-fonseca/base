cmake_minimum_required (VERSION 3.1)

project (testsuite)

#add_executable(version Version.cpp)
#target_link_libraries(version base)

file( GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp )
# TAG: add recursive
# TAG: fix all builds
list(REMOVE_ITEM TEST_SOURCES "validate.cpp" "sax.cpp" "objinfo.cpp" "inet.cpp" "touch.cpp" "dlapp.cpp" "dom.cpp" "referenceCounting.cpp" "coff.cpp" "xsltprocessor.cpp" "eval.cpp" "MyLinkerModule.cpp" "Function.cpp" "Map.cpp" "HashTable.cpp" "dlmodule.cpp")

foreach( sourcefile ${TEST_SOURCES} )
  string( REPLACE ".cpp" "" name ${sourcefile} )
  add_executable( ${name} ${sourcefile} )
  target_link_libraries( ${name} base )
endforeach()
