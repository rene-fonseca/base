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
endif ()

include_directories ("${PROJECT_SOURCE_DIR}")

add_library(base base/Date.cpp)
