#!/bin/bash

CLICOLOR=1
cmake $BUILD_SOURCESDIRECTORY -DCOLOR=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=install -DCMAKE_TOOLCHAIN_FILE=/wasi-sdk-8.0/share/cmake/wasi-sdk.cmake -DWASI_SDK_PREFIX=/wasi-sdk-8.0
# BUILD_BINARIESDIRECTORY

cmake --build . --config Debug --target install -- -j 4

PATH=/wasmtime:$PATH
wasmtime ./testsuite/unittesting --env=CLICOLOR_FORCE=1 --env=AGENT_BUILDDIRECTORY= -- --junit stderr 2> junit.xml
ctest -T Test
