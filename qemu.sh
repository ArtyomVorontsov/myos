#!/bin/sh

set -e
. ./iso.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom myos.iso -boot d -d int,cpu_reset,guest_errors -D qemu.log -monitor stdio -machine pc -m 128M -drive file=my_vm_disk.qcow2,if=ide -drive file=my_vm_disk2.qcow2,if=ide
