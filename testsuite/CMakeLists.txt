cmake_minimum_required (VERSION 3.1)

project(testsuite)

file(GLOB TEST_SOURCES RELATIVE "${PROJECT_SOURCE_DIR}" *.cpp)
list(REMOVE_ITEM TEST_SOURCES "MyLinkerModule.cpp" "Function.cpp" "dlapp.cpp" "OpenCL.cpp")
# typeinfo issue for dlapp

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (USE_SHARED)
  add_library(MyLinkerModule SHARED "MyLinkerModule.cpp")
  add_dependencies(MyLinkerModule base)
  if (0)
    target_link_libraries(MyLinkerModule base_STATIC)
    if (WIN32)
      target_link_libraries(MyLinkerModule Ws2_32 Netapi32 Winmm)
    endif ()
  else ()
    target_link_libraries(MyLinkerModule base)
    target_compile_definitions(MyLinkerModule PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
  endif ()
  set_target_properties(MyLinkerModule PROPERTIES FOLDER testsuite)
endif ()

foreach(sourcefile ${TEST_SOURCES})

  string(REPLACE ".cpp" "" name ${sourcefile})

  if (NOT USE_SHARED)
    if (${name} STREQUAL "dlmodule")
      continue ()
    endif ()
  endif ()

  if (${name} STREQUAL "dlmodule")
    add_library(${name} SHARED ${sourcefile})
    add_dependencies(${name} MyLinkerModule)
    target_link_libraries(${name} MyLinkerModule)
  else ()
    add_executable(${name} ${sourcefile})
  endif ()

  if (NOT USE_SHARED)
    add_dependencies(${name} base_STATIC)
    target_link_libraries(${name} base_STATIC)
    if (WIN32)
      target_link_libraries(${name} Ws2_32 Netapi32 Winmm)
    endif ()
  else () # use shared
    add_dependencies(${name} base)
    target_link_libraries(${name} base)
    target_compile_definitions(${name} PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY)
  endif ()

  set_target_properties(${name} PROPERTIES FOLDER testsuite)

# do NOT install all tests for WebAssembly platforms due to size
if (("${name}" STREQUAL "unittesting") OR NOT (EMSCRIPTEN OR ("${CMAKE_SYSTEM_NAME}" STREQUAL "Wasm") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "WASI")))
  install(TARGETS ${name}
    CONFIGURATIONS Debug
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/debug"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/debug"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/debug"
    BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/debug"
  )

  install(TARGETS ${name}
    CONFIGURATIONS Release
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/release"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/release"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/release"
    BUNDLE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/release"
  )
endif ()

endforeach()

# add_subdirectory(orb)
add_subdirectory(ui)

if (1)
  enable_testing()

set (EXTENSION "")
if (NOT CTEST_INTERPRETER)
  set (CTEST_INTERPRETER "")
endif ()
if (EMSCRIPTEN)
  set (EXTENSION ".js")
  set (CTEST_INTERPRETER node --experimental-wasm-threads --experimental-wasm-bulk-memory)
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Wasm")
  set (CTEST_INTERPRETER wasmtime)
endif ()
  
  # we run this separately add_test(NAME unittesting COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/unittesting${EXTENSION})
  add_test(NAME Array COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Array${EXTENSION})
  add_test(NAME BitSet COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/BitSet${EXTENSION})
  add_test(NAME HashSet COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/HashSet${EXTENSION})
  add_test(NAME HashTable COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/HashTable${EXTENSION})
  add_test(NAME JSON COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/JSON${EXTENSION})
  add_test(NAME List COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/List${EXTENSION})
  add_test(NAME Map COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Map${EXTENSION})
  add_test(NAME ObjectModel COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/ObjectModel${EXTENSION})
  add_test(NAME Queue COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Queue${EXTENSION})
  add_test(NAME Set COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Set${EXTENSION})
  add_test(NAME Stack COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Stack${EXTENSION})
  add_test(NAME Date COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Date${EXTENSION})
  add_test(NAME String COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/String${EXTENSION})
  add_test(NAME Thread COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/Thread${EXTENSION})
  add_test(NAME demangle COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/demangle${EXTENSION})
  add_test(NAME floatingPoint COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/floatingPoint${EXTENSION})
  add_test(NAME io COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/io${EXTENSION})
  add_test(NAME uuid COMMAND ${CTEST_INTERPRETER} ${CMAKE_CURRENT_BINARY_DIR}/uuid${EXTENSION})

if (WIN32)
  # TAG: need to set PATH to find DLL and use Debug/Release subfolder
  # set_tests_properties(unittesting PROPERTIES ENVIRONMENT "PATH=${CMAKE_BINARY_DIR};$ENV{PATH}")
endif ()
endif ()
