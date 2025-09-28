#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd $SCRIPT_DIR

cd ../kernel
make
cd -

../iso-scripts/make-iso.sh

# cleanup
cd ../kernel
make cleanup
cd -

# run os with qemu
./run-myos.sh

# cleanup
rm -rf ../script-temp