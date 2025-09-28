#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd $SCRIPT_DIR

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
cp ../kernel/build/myos.bin isodir/boot/myos.bin
echo "menuentry \"myos\" {" >> isodir/boot/grub/grub.cfg
echo	"multiboot /boot/myos.bin" >> isodir/boot/grub/grub.cfg
echo "}" >> isodir/boot/grub/grub.cfg

cd -
grub-mkrescue -o myos.iso $SCRIPT_DIR/isodir
rm -r $SCRIPT_DIR/isodir

