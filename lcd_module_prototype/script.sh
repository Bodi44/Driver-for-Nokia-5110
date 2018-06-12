#!/bin/bash
{
make
insmod char_driver.ko 
make clean 
} &> /dev/null
dmesg | tail -1
