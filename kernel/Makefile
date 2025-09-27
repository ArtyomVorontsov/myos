
# sudo apt-get install g++ binutils libc6-dev-i386


GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o

%.o: %.cpp
	$(HOME)/opt/cross/bin/$(TARGET)-gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	$(HOME)/opt/cross/bin/$(TARGET)-as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	$(HOME)/opt/cross/bin/$(TARGET)-ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin