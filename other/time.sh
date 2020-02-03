#!/bin/bash

# echo $(CC) time.c -o gettime

# TAG: extract source file from command [ ][^ ]*/base/.*\.cpp

START=`./gettime`
#START=`python -c 'import time; print(time.time())'`
sleep .1
#END=`python -c 'import time; print(time.time())'`
END=`./gettime`

#echo $START
#echo $END

DIFF=`bc <<< "$END - $START"`
echo $DIFF
