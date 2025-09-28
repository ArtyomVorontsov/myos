#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd $SCRIPT_DIR
cd ..
qemu-system-i386 -cdrom ./script-temp/myos.iso


