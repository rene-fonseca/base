cmake_minimum_required (VERSION 3.1)

project (testsuite)

file( GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp )
# list(REMOVE_ITEM TEST_SOURCES "")

foreach( sourcefile ${TEST_SOURCES} )
  string( REPLACE ".cpp" "" name ${sourcefile} )
  add_executable( ${name} ${sourcefile} )
  
  if (NOT USE_SHARED)
    add_dependencies(${name} base_STATIC)
    target_link_libraries(${name} base_STATIC)
    if (MSVC)
      target_link_libraries(${name} Ws2_32 Netapi32 Winmm)
    endif ()
  else () # use shared
    add_dependencies(${name} base)
    target_link_libraries(${name} base)
    target_compile_definitions(${name} PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
  endif ()

  set_target_properties(${name} PROPERTIES FOLDER testsuite)
endforeach()
