#!/bin/bash

# The following program can be run 5 times, with values of i from 0 to 4
# One can then do `watch "ps aux | grep 4.sh | wc -l"` to see if one has halted.
# A bash script `cuz I'm too lazy

i=4;
while [ "000000" != "$(printf "ckczppom$i" | md5sum | cut -c1-6 )" ]
do
echo "not $i"
i=$((5 + $i))
done
echo $i
