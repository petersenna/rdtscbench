#!/bin/bash

# Peter Senna Tschudin - peter.senna@gmail.com

clear
echo
echo "BEFORE RUNNING MAKE: "
echo "To improve accuracy, disable cpu scalling with: # service cpuspeed stop"
echo "Having constant_tsc flag at /proc/cpuinfo also improves the accuracy."
echo "This is a dumb message that is shown everytime. :-)"
echo

rm -rf ./hzdef.h

MHZ=`cat /proc/cpuinfo |grep MHz |head -n 1|cut -d ":" -f 2| cut -d " " -f 2|cut -d "." -f 1`

echo -n "#define CPU_HZ $MHZ" > ./hzdef.h
echo -n "000000LL" >> ./hzdef.h
echo >> ./hzdef.h

