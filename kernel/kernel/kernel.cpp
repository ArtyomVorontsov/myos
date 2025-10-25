#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
#include <kernel/keyboard.hpp>

extern "C" void kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt);
	KeyboardDriver keyboardDrive(&interrupts);

	interrupts.Activate();

	while (1)
		;
}
