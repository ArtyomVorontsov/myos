#!/bin/sh

set -e

qemu-system-x86_64 -cdrom CorePure64-16.2.iso -boot d -d int,cpu_reset -machine pc -m 128M -hda tinycoredisk.qcow2 -hdb my_vm_disk.qcow2
