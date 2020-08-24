#!/bin/bash

# The following program can be run 5 times, with values of i from 0 to 4
# One can then do `watch "ps aux | grep 4.sh | wc -l"` to see if one has halted.
# A bash script `cuz I'm too lazy

i=$1;
echo $i;
j=0;
while [ "$j" -le 8 ]
do
while [ "00000" != "$(printf "ffykfhsq$i" | md5sum | cut -c1-6 )" ]
do
#echo "not $i"
i=$((8 + $i))
done
echo $i
printf "ffykfhsq$i" | md5sum
j=$(($j+1))
echo $j
i=$((8 + $i))
done
