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
  # TAG: need to fix all exception specifications
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290 /wd4838 /wd4244 /wd4267")
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
  "base/collection/*.h"
  "base/collection/*.cpp"
)

# message(${base_SRC})

add_library(base ${base_SRC} base/Date.cpp base/UnsignedInteger.cpp base/UnsignedLongInteger.cpp base/ValidifiedResult.cpp base/Version.cpp)
