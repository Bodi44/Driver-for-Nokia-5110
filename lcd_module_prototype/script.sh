#!/bin/bash
make
insmod project.ko
dmesg | tail -n
make clean
