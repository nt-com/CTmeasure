#!/bin/sh

########################################################
# nmt 2016
# flash script for ATMEL bare metal programming
#
########################################################

clear

echo
echo -!- FLASH SCRIPT -!-
echo

read -p "name of program to flash -> " name
echo .....................
echo -- FLASHING $name --
echo .....................
echo

avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB1 -b 57600 -U flash:w:$name.hex

#avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$name.hex
