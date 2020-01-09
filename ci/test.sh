#!/bin/bash

CLICOLOR=1
./testsuite/unittesting --junit junit.xml
ctest -T Test
