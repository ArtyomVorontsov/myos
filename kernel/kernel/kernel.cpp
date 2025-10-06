#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>	

extern "C" void kernel_main(void)
{
	GlobalDescriptorTable gdt;
	terminal_initialize();
	printf("Hello, MYOS C++ kernel!!!\n");
}
