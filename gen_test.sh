#!/bin/bash
numberfile=test/nums
i=$1
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile 
done
i=$1
numberfile=test/num
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile
done
i=$1
numberfile=test/numss
echo $i > $numberfile
while [ $i -ge -$1 ]; do
  ((i=i-1))
  echo $i >> $numberfile
done

