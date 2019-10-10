cmake_minimum_required (VERSION 3.1)

# see https://cmake.org/cmake-tutorial/

# set(CMAKE_VERBOSE_MAKEFILE 1)

project (base)

set (base_VERSION_MAJOR 0)
set (base_VERSION_MINOR 9)

#configure_file (
#  "${PROJECT_SOURCE_DIR}/configure.h.in"
#  "${PROJECT_BINARY_DIR}/configure.h"
#)

if (MSVC)
  add_definitions(-DUNICODE)
  add_definitions(-D_UNICODE)
endif ()

if (MSVC)
  # TAG: need to fix all exception specifications
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290 /wd4838 /wd4244 /wd4267 /wd4996 /wd4297 /wd4251")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exceptions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") # no warnings allowed
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated") # exception specifications
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overflow") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated-declarations") # FIXME
endif ()

endif ()

include_directories ("${PROJECT_SOURCE_DIR}")

file(GLOB base_SRC
  "base/*.h"
  "base/*.cpp"
  "base/archive/*.h"
  "base/archive/*.cpp"
  "base/collection/*.h"
  "base/collection/*.cpp"
  "base/concurrency/*.h"
  "base/concurrency/*.cpp"
  "base/communication/*.h"
  "base/communication/*.cpp"
  "base/compression/*.h"
  "base/compression/*.cpp"
  "base/dl/*.h"
  "base/dl/*.cpp"
  "base/filesystem/*.h"
  "base/filesystem/*.cpp"
  "base/io/*.h"
  "base/io/*.cpp"
  "base/io/async/*.h"
  "base/io/async/*.cpp"
  "base/iterator/*.h"
  "base/iterator/*.cpp"
  "base/mathematics/*.h"
  "base/mathematics/*.cpp" # TAG: rename to math
  "base/mem/*.h"
  "base/mem/*.cpp"
  "base/net/*.h"
  "base/net/*.cpp"
  "base/opengl/*.h"
  "base/opengl/*.cpp"
  "base/platforms/*.h"
  "base/platforms/*.cpp"
  "base/rmi/*.h"
#  "base/rmi/*.cpp"
  "base/rmi/idl/*.h"
#  "base/rmi/idl/*.cpp"
  "base/security/*.h"
  "base/security/*.cpp"
  "base/sound/*.h"
  "base/sound/*.cpp" # TAG: rename to audio?
  "base/string/*.h"
  "base/string/*.cpp"
  "base/ui/*.h"
  "base/ui/*.cpp"
  "base/xml/*.h"
  "base/xml/*.cpp"
)

execute_process(OUTPUT_VARIABLE GIT_BRANCH COMMAND git symbolic-ref --short HEAD)
string(REGEX REPLACE "\n$" "" GIT_BRANCH "${GIT_BRANCH}")
execute_process(OUTPUT_VARIABLE GIT_COMMIT COMMAND git rev-parse HEAD)
string(REGEX REPLACE "\n$" "" GIT_COMMIT "${GIT_COMMIT}")
execute_process(OUTPUT_VARIABLE GIT_COMMIT_SHORT COMMAND git rev-parse --short HEAD)
string(REGEX REPLACE "\n$" "" GIT_COMMIT_SHORT "${GIT_COMMIT_SHORT}")
execute_process(OUTPUT_VARIABLE GIT_REVISION COMMAND git rev-list HEAD --count)
string(REGEX REPLACE "\n$" "" GIT_REVISION "${GIT_REVISION}")

message("Branch: ${GIT_BRANCH}")
message("Commit: ${GIT_COMMIT}")
message("Revision: ${GIT_REVISION}")

# generate configuration header
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/generated/config.h)
include_directories(${CMAKE_BINARY_DIR}/generated)

#add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/base_config.h
#  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/base_config.h)

add_library(base SHARED ${base_SRC} ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h)
if (MSVC)
target_compile_definitions(base PUBLIC _DK_SDU_MIP__BASE__SHARED_LIBRARY_BUILD)
# set_target_properties(base PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
target_link_libraries(base Ws2_32 Netapi32 Winmm)
elseif (UNIX AND NOT APPLE)
target_link_libraries(base pthread X11)
endif ()

add_library(base_STATIC STATIC ${base_SRC} ${CMAKE_CURRENT_BINARY_DIR}/generated/config.h)
set_target_properties(base_STATIC PROPERTIES OUTPUT_NAME base_static)

# avoid no symbols warnings
if (APPLE)
set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif ()

add_subdirectory("testsuite")
