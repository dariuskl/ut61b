#!/bin/bash
# based on http://erste.de/UT61/index.html
for dat in /sys/bus/usb/devices/*; do
  if test -e $dat/idVendor; then
    grep "1a86" $dat/idVendor > /dev/null && grep "e008" $dat/idProduct > /dev/null && echo $dat && echo auto > ${dat}/power/level && echo 0 > ${dat}/power/autosuspend
  fi
done
