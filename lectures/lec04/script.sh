#!/bin/sh
langs="ar de en es fr"
for l1 in $langs; do
    for l2 in $langs; do
        echo $l1 $l2
        # gcc -o $1-$2 $l1-e.c $l2-e.c
    done
done
