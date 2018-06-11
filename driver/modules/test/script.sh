#!/bin/bash
make
sudo insmod ./project.ko
make clean
