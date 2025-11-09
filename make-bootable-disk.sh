#!/bin/sh

FILE=myos.iso

if [[ $# -eq 0 ]]; then 
    echo "param 1 is missing";
    exit 1;
fi 

if [[ $1 == "sda" ]]; then
    echo "sda is system disk, be careful!";
    exit 1;
fi


if [ ! -f "$FILE" ]; then
    echo "file $FILE does not exist, please create that with make-iso.sh";
    exit 1;
fi

sudo dd if=$FILE of=/dev/$1 && sync