#!/bin/bash
 
# created     : 2019/03/19
# last update : 2019/03/19
# author      : breno <brenoferrazdeoliveira@gmail.com>
# notes       : 

gcc -lm -lgsl -lgslcblas ising-mc.c
./a.out
