cmake_minimum_required (VERSION 3.1)

project(testsuite)

file(GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp)
# TAG: add recursive
# TAG: fix all builds
list(REMOVE_ITEM TEST_SOURCES "MyLinkerModule.cpp" "Function.cpp" "dlmodule.cpp2")

# message("${CMAKE_CURRENT_BINARY_DIR}/../base" )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(MyLinkerModule SHARED "MyLinkerModule.cpp")
add_dependencies(MyLinkerModule base base_STATIC)
if (0)
  target_link_libraries(MyLinkerModule "${CMAKE_CURRENT_BINARY_DIR}/../DEBUG/base_static.lib")
  target_link_libraries(MyLinkerModule Ws2_32 Netapi32 Winmm)
else ()
  target_link_libraries(MyLinkerModule base)
endif ()

foreach(sourcefile ${TEST_SOURCES})
  
  string(REPLACE ".cpp" "" name ${sourcefile})
if (${name} STREQUAL "dlmodule")
  add_library(${name} SHARED ${sourcefile})
  add_dependencies(${name} base MyLinkerModule)
  target_compile_definitions(${name} PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
  target_link_libraries(${name} base MyLinkerModule)
else ()
  add_executable(${name} ${sourcefile})
  target_compile_definitions(${name} PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
endif ()
  add_dependencies(${name} base base_STATIC)
#if (MSVC)
#  target_link_libraries(${name} "${CMAKE_CURRENT_BINARY_DIR}/../DEBUG/base_static.lib")
#  target_link_libraries(${name} Ws2_32 Netapi32 Winmm)
#else ()
  target_link_libraries(${name} base)
#endif ()

  set_target_properties(${name} PROPERTIES FOLDER testsuite)
endforeach()

# add_subdirectory(orb)
add_subdirectory(ui)
