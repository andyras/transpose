#!/bin/bash

for testmat in $(ls test/*mat)
do
 echo "Testing file: ${testmat}"
 ./transpose -d $testmat
done
