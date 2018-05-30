#!/bin/bash
make
sudo insmod $1
make clean
sudo rmmod $1

