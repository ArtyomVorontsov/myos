#!/bin/bash

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
cp kernel/build/myos.bin isodir/boot/myos.bin
echo "menuentry \"myos\" {" >> isodir/boot/grub/grub.cfg
echo	"multiboot /boot/myos.bin" >> isodir/boot/grub/grub.cfg
echo "}" >> isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir

