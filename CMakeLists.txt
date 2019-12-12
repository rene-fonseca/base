cmake_minimum_required(VERSION 3.1)

# see https://cmake.org/cmake-tutorial/

# set(CMAKE_VERBOSE_MAKEFILE 1)

project(base)

set(base_VERSION_MAJOR 0)
set(base_VERSION_MINOR 9)
set(base_VERSION_MICRO 0)

if (MSVC)
  add_definitions(-DUNICODE)
  add_definitions(-D_UNICODE)
endif ()

message("System: ${CMAKE_SYSTEM_NAME}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_COM_AZURE_DEV__BASE__DEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} xxx")

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8") # force UTF-8 encoding for compiler
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # build in parallel
  if ("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "AMD64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc") # required for AMD64
  endif ()

  # TAG: need to fix all exception specifications
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # no warnings allowed
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4290 /wd4244 /wd4267 /wd4297")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") # deprecated
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # DLL export
else ()

  if ("${CMAKE_SYSTEM}" MATCHES "Linux")
    if (EXISTS "/etc/os-release")
      file(READ "/etc/os-release" OS_RELEASE_FILE)
      string(REGEX MATCH "[\r\n]ID=([a-zA-Z0-9 /\\.]+)" _ ${OS_RELEASE_FILE})
      set(OS_RELEASE_ID ${CMAKE_MATCH_1})
    endif ()
  endif ()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable") # ignore unused variables
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field") # ignore unused field
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exceptions")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") # no warnings allowed
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # can be used to check for exception spec issues -fno-enforce-eh-specs
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated") # exception specifications
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overflow") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated-declarations") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=terminate") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-terminate") # TAG: critical to be fixed
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations") # FIXME
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") # no warnings allowed

  if ("${OS_RELEASE_ID}" STREQUAL "raspbian")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi") # parameter passing for argument of changed
  endif ()
endif ()

endif ()

if (0)
find_package(OpenGL REQUIRED)

if (OPENGL_FOUND)
  message("Found OpenGL for macOS")
  message("OpenGL for macOS, include dir: ${OPENGL_INCLUDE_DIR}")
  message("OpenGL for macOS, link libraries: ${OPENGL_gl_LIBRARY}")
else (OPENGL_FOUND)
  message(FATAL_ERROR "OpenGL for macOS not found")
endif()

#target_link_libraries(targets
#  ${OPENGL_gl_LIBRARY}
#  ${OPENGL_glu_LIBRARY}
#)
endif ()

if (UNIX)
  find_package(X11)
endif()

find_package(ZLib)
find_package(BZip2)

file(GLOB_RECURSE base_platforms_SRC
  "base/platforms/*.h"
  "base/platforms/*.cpp"
)

list(FILTER base_platforms_SRC EXCLUDE REGEX "^.*\/base\/platforms\/win32\/.+")

# GLOB_RECURSE
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
  "base/math/*.h"
  "base/math/*.cpp"
  "base/mem/*.h"
  "base/mem/*.cpp"
  "base/net/*.h"
  "base/net/*.cpp"
  "base/objectmodel/*.h"
  "base/objectmodel/*.cpp"
#  "base/opencl/*.h"
#  "base/opencl/*.cpp"
  "base/opengl/*.h"
  "base/opengl/*.cpp"
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
  "base/webassembly/*.h"
  "base/webassembly/*.cpp"
  "base/xml/*.h"
  "base/xml/*.cpp"
)
list(APPEND base_SRC ${base_platforms_SRC})

foreach(source IN ITEMS ${base_SRC})
  get_filename_component(source_path "${source}" PATH)
  file(RELATIVE_PATH source_path_rel "${PROJECT_SOURCE_DIR}/base" "${source_path}")
  string(REPLACE "/" "\\" group_path "${source_path_rel}")
  source_group("${group_path}" FILES "${source}")
  #message("${group_path} -> ${source}")
endforeach()

execute_process(OUTPUT_VARIABLE GIT_BRANCH COMMAND git symbolic-ref --short HEAD)
string(REGEX REPLACE "\n$" "" GIT_BRANCH "${GIT_BRANCH}")
execute_process(OUTPUT_VARIABLE GIT_COMMIT COMMAND git rev-parse HEAD)
string(REGEX REPLACE "\n$" "" GIT_COMMIT "${GIT_COMMIT}")
execute_process(OUTPUT_VARIABLE GIT_COMMIT_SHORT COMMAND git rev-parse --short HEAD)
string(REGEX REPLACE "\n$" "" GIT_COMMIT_SHORT "${GIT_COMMIT_SHORT}")
execute_process(OUTPUT_VARIABLE GIT_REVISION COMMAND git rev-list HEAD --count)
string(REGEX REPLACE "\n$" "" GIT_REVISION "${GIT_REVISION}")

string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)
string(TIMESTAMP BUILD_DATE_SECONDS "%s" UTC)

message("Branch: ${GIT_BRANCH}")
message("Commit: ${GIT_COMMIT}")
message("Revision: ${GIT_REVISION}")

# generate configuration header
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/generated/base/config.h)

if (ZLIB_FOUND)
option(_COM_AZURE_DEV__BASE__USE_ZLIB "Enable ZLib" ON)
endif ()
if (BZIP2_FOUND)
option(_COM_AZURE_DEV__BASE__USE_BZIP2 "Enable BZip2" ON)
endif ()

option(_COM_AZURE_DEV__BASE__USE_X11 "Enable X11" X11_FOUND)
option(_COM_AZURE_DEV__BASE__USE_WASM "Enable WASMTIME" WASMTIME_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/build.h.in ${CMAKE_BINARY_DIR}/generated/base/build.h)
# generate header with include of ZLIB_INCLUDE_DIRS
# generate header with include of BZIP2_INCLUDE_DIR
# e.g. #include <relativepath/xxx.h> ${CMAKE_BINARY_DIR}/generated/base/bzip2.h)

set(base_ADDITIONAL "")
if (MSVC)
  configure_file(${CMAKE_SOURCE_DIR}/base.rc.in ${CMAKE_BINARY_DIR}/generated/base/base.rc)
  set(base_ADDITIONAL ${CMAKE_BINARY_DIR}/generated/base/base.rc)
endif ()

add_library(base SHARED ${base_SRC} ${base_ADDITIONAL} ${CMAKE_CURRENT_BINARY_DIR}/generated/base/config.h)
target_compile_definitions(base PRIVATE _COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)

# find_package(OpenCL REQUIRED)

if (1)
target_include_directories(base PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/base> # <prefix>/include
)
target_include_directories(base PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
  $<INSTALL_INTERFACE:include/base> # <prefix>/include
)
endif ()

if (MSVC)
  target_link_libraries(base Ws2_32 Netapi32 Winmm Wininet Ole32 Gdi32 Advapi32)
elseif (APPLE)
  find_library(corefoundation CoreFoundation)
  find_library(cfnetwork CFNetwork)
  target_link_libraries(base ${cfnetwork} ${corefoundation})
elseif (UNIX)
  target_link_libraries(base pthread)
  if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "OpenBSD")
    target_link_libraries(base dl)
  endif ()
  if (X11_FOUND)
    target_link_libraries(base ${X11_X11_LIB})
  endif ()
# elseif (APPLE)
#  target_link_libraries(base pthread ${OpenCL_LIBRARY})
endif ()

if (ZLIB_FOUND)
  target_link_libraries(base ${ZLIB_LIBRARIES})
endif ()
if (BZIP2_FOUND)
  target_link_libraries(base ${BZIP2_LIBRARIES})
endif ()


# macOS requires all values to be less than 1024
set_target_properties(base
  PROPERTIES
  VERSION ${base_VERSION_MAJOR}.${base_VERSION_MINOR}
  SOVERSION ${base_VERSION_MAJOR})

add_library(base_STATIC STATIC ${base_SRC} ${CMAKE_CURRENT_BINARY_DIR}/generated/base/config.h)
set_target_properties(base_STATIC PROPERTIES OUTPUT_NAME base_static)

if (1)
target_include_directories(base_STATIC PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/base> # <prefix>/include
)
target_include_directories(base_STATIC PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
  $<INSTALL_INTERFACE:include/base> # <prefix>/include
)
endif ()

file(GLOB_RECURSE base_HEADERS
  "base/*.h"
)

set_target_properties(base PROPERTIES PUBLIC_HEADER "${base_HEADERS}")

if (1)
# preserve folder structure
install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/base" # source directory
  DESTINATION "${CMAKE_BINARY_DIR}/install/include" # target directory
  FILES_MATCHING PATTERN "*.h" # select header files
)
install(
  DIRECTORY "${CMAKE_BINARY_DIR}/generated/base" # source directory
  DESTINATION "${CMAKE_BINARY_DIR}/install/include" # target directory
  FILES_MATCHING PATTERN "*.h" # select header files
)
endif ()

# install(FILES ${base_HEADERS} DESTINATION include/base)

# set(CMAKE_DEBUG_POSTFIX "-debug")
### install(TARGETS base base_STATIC
###   EXPORT baseConfig
###   PUBLIC_HEADER DESTINATION "${CMAKE_BINARY_DIR}/install/include/base"
### )

install(TARGETS base base_STATIC
  CONFIGURATIONS Debug
  EXPORT baseConfigDebug
  PUBLIC_HEADER DESTINATION "${CMAKE_BINARY_DIR}/install/include/base"
  ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/install/lib/debug"
  LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/install/lib/debug"
  RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/install/bin/debug"
)

install(TARGETS base base_STATIC
  CONFIGURATIONS Release
  EXPORT baseConfig
  PUBLIC_HEADER DESTINATION "${CMAKE_BINARY_DIR}/install/include/base"
  ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}/install/lib/release"
  LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/install/lib/release"
  RUNTIME DESTINATION "${CMAKE_BINARY_DIR}/install/bin/release"
)

if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
  list(APPEND CMAKE_PROGRAM_PATH "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif ()

if (0) # TAG: not working yet
export(TARGETS base base_STATIC
  NAMESPACE base::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/baseConfig.cmake"
)

install(EXPORT baseConfigDebug
  DESTINATION "${CMAKE_BINARY_DIR}/install/lib/cmake/base"
  NAMESPACE base::
)
install(EXPORT baseConfig
  DESTINATION "${CMAKE_BINARY_DIR}/install/lib/cmake/base"
  NAMESPACE base::
)
endif ()

#install(
#  DIRECTORY "${CMAKE_BINARY_DIR}/Release"
#  CONFIGURATIONS Release
#  DESTINATION "${CMAKE_BINARY_DIR}/bin/release"
#  FILES_MATCHING
#  PATTERN *.pdb
#)

add_subdirectory("testsuite")

include(CTest)
enable_testing()

if (CMAKE_VERSION VERSION_GREATER 3.7.8)
  if (MSVC_IDE)
    option(VS_ADD_NATIVE_VISUALIZERS "Configure project to use Visual Studio native visualizers" TRUE)
  else()
    set(VS_ADD_NATIVE_VISUALIZERS FALSE CACHE INTERNAL "Native visualizers are Visual Studio extension" FORCE)
  endif()
  if (VS_ADD_NATIVE_VISUALIZERS)
     target_sources(base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/base.natvis)
     target_sources(base_STATIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/base.natvis)
  endif()
endif()
