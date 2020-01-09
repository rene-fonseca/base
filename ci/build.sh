#!/bin/bash

CLICOLOR=1
cmake --build . --config Debug --target install -- -j 4
