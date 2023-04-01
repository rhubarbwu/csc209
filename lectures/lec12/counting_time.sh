#!/bin/sh
S=1073741824
T=1

for i in {0..14}; do
    echo $i: using $T thread\(s\) each counting $S
    if [ -v $TIME ]; then
        time $1 $T $S $1
    else
        $1 $T $S $2
    fi
    S=$(expr $S / 2)
    T=$(expr $T \* 2)
done
