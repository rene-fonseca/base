cmake_minimum_required (VERSION 3.1)

project(testsuite)

file(GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp)
list(REMOVE_ITEM TEST_SOURCES "MyLinkerModule.cpp" "Function.cpp" "dlapp.cpp" "OpenCL.cpp")
# typeinfo issue for dlapp

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(MyLinkerModule SHARED "MyLinkerModule.cpp")
add_dependencies(MyLinkerModule base)
if (0)
  target_link_libraries(MyLinkerModule base_STATIC)
  target_link_libraries(MyLinkerModule Ws2_32 Netapi32 Winmm)
else ()
  target_link_libraries(MyLinkerModule base)
  target_compile_definitions(MyLinkerModule PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
endif ()
set_target_properties(MyLinkerModule PROPERTIES FOLDER testsuite)

foreach(sourcefile ${TEST_SOURCES})
  
  string(REPLACE ".cpp" "" name ${sourcefile})
if (${name} STREQUAL "dlmodule")
  add_library(${name} SHARED ${sourcefile})
  add_dependencies(${name} MyLinkerModule)
  target_link_libraries(${name} MyLinkerModule)
else ()
  add_executable(${name} ${sourcefile})
endif ()

if (0 AND MSVC) # easier to test with static build
  add_dependencies(${name} base_STATIC)
  target_link_libraries(${name} base_STATIC)
  target_link_libraries(${name} Ws2_32 Netapi32 Winmm)
else () # use shared
  add_dependencies(${name} base)
  target_link_libraries(${name} base)
  target_compile_definitions(${name} PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
endif ()

  set_target_properties(${name} PROPERTIES FOLDER testsuite)

  install(TARGETS ${name}
    CONFIGURATIONS Debug
    ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/install/bin/debug"
    LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/install/bin/debug"
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/install/bin/debug"
  )

  install(TARGETS ${name}
    CONFIGURATIONS Release
    ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/install/bin/release"
    LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/install/bin/release"
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/install/bin/release"
  )

endforeach()

# add_subdirectory(orb)
add_subdirectory(ui)

if (1)
  enable_testing()
  
  # TAG: need to export unittesting results
  add_test(NAME unittesting COMMAND ${CMAKE_CURRENT_BINARY_DIR}/unittesting)
  add_test(NAME Array COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Array)
  add_test(NAME HashSet COMMAND ${CMAKE_CURRENT_BINARY_DIR}/HashSet)
  add_test(NAME HashTable COMMAND ${CMAKE_CURRENT_BINARY_DIR}/HashTable)
  add_test(NAME List COMMAND ${CMAKE_CURRENT_BINARY_DIR}/List)
  add_test(NAME Map COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Map)
  add_test(NAME Stack COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Stack)
  add_test(NAME Date COMMAND ${CMAKE_CURRENT_BINARY_DIR}/Date)
  add_test(NAME String COMMAND ${CMAKE_CURRENT_BINARY_DIR}/String)
  
if (MSVC)
  # TAG: need to set PATH to find DLL and use Debug/Release subfolder
  # set_tests_properties(unittesting PROPERTIES ENVIRONMENT "PATH=${CMAKE_BINARY_DIR};$ENV{PATH}")
endif ()
endif ()
