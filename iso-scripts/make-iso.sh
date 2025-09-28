#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd $SCRIPT_DIR

mkdir -p $SCRIPT_DIR/../script-temp

cd $SCRIPT_DIR/../script-temp

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
cp ../kernel/build/myos.bin isodir/boot/myos.bin
echo "menuentry \"myos\" {" >> isodir/boot/grub/grub.cfg
echo	"multiboot /boot/myos.bin" >> isodir/boot/grub/grub.cfg
echo "}" >> isodir/boot/grub/grub.cfg

grub-mkrescue -o $SCRIPT_DIR/../script-temp/myos.iso $SCRIPT_DIR/../script-temp/isodir

