cmake_minimum_required (VERSION 3.1)

project(testsuite)

file(GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp)
list(REMOVE_ITEM TEST_SOURCES "MyLinkerModule.cpp" "Function.cpp" "dlapp.cpp" "OpenCL.cpp")
# typeinfo issue for dlapp

# message("${CMAKE_CURRENT_BINARY_DIR}/../base" )
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
    ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/bin/debug"
    LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/bin/debug"
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/bin/debug"
  )

  install(TARGETS ${name}
    CONFIGURATIONS Release
    ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/bin/release"
    LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/bin/release"
    RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/bin/release"
  )

endforeach()

# add_subdirectory(orb)
add_subdirectory(ui)
