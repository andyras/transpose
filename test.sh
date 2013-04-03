#!/bin/bash
# This script tests transpose on various inputs, some of which will
# cause an error and some of which will run fine.

for testmat in $(ls test/*mat)
do
 echo "Testing file: ${testmat}"
 ./transpose -d $testmat
done
