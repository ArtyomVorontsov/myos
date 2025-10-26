#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
#include <kernel/keyboard.hpp>
#include <kernel/mouse.hpp>

extern "C" void kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt);
	MouseDriver mouse(&interrupts);
	KeyboardDriver keyboardDrive(&interrupts);

	interrupts.Activate();

	while (1)
		;
}
