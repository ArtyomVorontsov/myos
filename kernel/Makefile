GPPPARAMS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDPARAMS = -melf_i386
TARGET = i686-elf

objects = boot.o kernel.o

%.o: %.cpp
	mkdir -p ./build
	$(HOME)/opt/cross/bin/$(TARGET)-g++ $(GPPPARAMS) -c -o build/$@ $<

%.o: %.s
	$(HOME)/opt/cross/bin/$(TARGET)-as $(ASPARAMS) -o build/$@ $<

myos.bin: linker.ld $(objects)
	$(HOME)/opt/cross/bin/$(TARGET)-gcc -ffreestanding -O2 -nostdlib ./build/boot.o ./build/kernel.o -lgcc -T linker.ld -o ./build/myos.bin  

install: myos.bin
	sudo cp $< /boot/myos.bin

cleanup: 
	rm -f *.out *.o