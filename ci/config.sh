#!/bin/bash

CLICOLOR=1
cd
set
cmake . -DCOLOR=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=install
