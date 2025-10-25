#!/bin/sh

set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom myos.iso -d int,cpu_reset -D qemu.log -monitor stdio -machine pc -m 128M