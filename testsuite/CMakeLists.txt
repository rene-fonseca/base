cmake_minimum_required (VERSION 3.1)

project (testsuite)

#add_executable(version Version.cpp)
#target_link_libraries(version base)

file( GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp )
# TAG: add recursive
# TAG: fix all builds
list(REMOVE_ITEM TEST_SOURCES "sax.cpp" "dom.cpp" "xsltprocessor.cpp" "MyLinkerModule.cpp" "Function.cpp" "dlapp.cpp" "dlmodule.cpp")
# TAG: MyLinkerModule

message ("${CMAKE_CURRENT_BINARY_DIR}/../base" )

foreach( sourcefile ${TEST_SOURCES} )
  string( REPLACE ".cpp" "" name ${sourcefile} )
  add_executable( ${name} ${sourcefile} )
  # add_dependencies( ${name} base )
  # target_compile_definitions( ${name} PUBLIC _DK_SDU_MIP__BASE__SHARED_LIBRARY )
  # target_link_libraries( ${name} "${CMAKE_CURRENT_BINARY_DIR}/../DEBUG/base.lib" )
  add_dependencies( ${name} base_STATIC )
if (MSVC)
  target_link_libraries( ${name} "${CMAKE_CURRENT_BINARY_DIR}/../DEBUG/base_static.lib" )
  target_link_libraries( ${name} Ws2_32 Netapi32 Winmm)
else ()
  target_link_libraries( ${name} base )
endif ()
endforeach()

add_subdirectory(ui)
