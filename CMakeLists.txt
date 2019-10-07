cmake_minimum_required (VERSION 3.1)

# see https://cmake.org/cmake-tutorial/

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290 /wd4838 /wd4244 /wd4267 /wd4996 /wd4297")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exceptions") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security") # FIXME
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
#  "base/opengl/*.h"
#  "base/opengl/*.cpp"
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
#  "base/ui/*.cpp"
  "base/xml/*.h"
#  "base/xml/*.cpp"
)

# message(${base_SRC})

add_library(base SHARED ${base_SRC})
add_library(base_STATIC STATIC ${base_SRC})
set_target_properties(base_STATIC PROPERTIES OUTPUT_NAME base_static)

if (MSVC)
target_link_libraries(base Ws2_32 Netapi32 Winmm)
endif ()

add_subdirectory("testsuite")
