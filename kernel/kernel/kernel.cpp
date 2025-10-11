#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
extern "C" void kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel!!!\n");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt);

	interrupts.Activate();

	while (1)
		;
}
