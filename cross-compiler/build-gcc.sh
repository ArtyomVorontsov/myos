export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd $HOME/src &&
echo "$HOME/src" >> progress.log &&
 The $PREFIX/bin dir _must_ be in the PATH. We did that above.
 which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc &&
echo "dir created" >> progress.log &&
cd build-gcc &&
../gcc-11.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx &&
echo "configuration completed" >> progress.log &&
make -j$(nproc) all-gcc &&
echo "all-gcc completed" >> progress.log &&
make -j$(nproc) all-target-libgcc &&
echo "all-target-libgcc completed" >> progress.log &&
make -j$(nproc) all-target-libstdc++-v3 &&
echo "all-target-libstdc++-v3 completed" >> progress.log &&
make -j$(nproc) install-gcc &&
echo "install-gcc completed" >> progress.log &&
make -j$(nproc) install-target-libgcc &&
echo "install-target-libgcc completed" >> progress.log &&
make -j$(nproc) install-target-libstdc++-v3
echo "completed" >> progress.log 

