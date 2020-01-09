#!/bin/bash

CLICOLOR=1
cmake /src -DCOLOR=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=install
cmake --build . --config Debug --target install -- -j 4
