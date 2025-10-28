#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
#include <driver/keyboard.hpp>
#include <driver/mouse.hpp>
#include <driver/driver.hpp>
#include <driver/driver-manager.hpp>
#include <event-handler/print-keyboard-event-handler.hpp>
#include <event-handler/print-mouse-event-handler.hpp>

extern "C" void
kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel\n");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt);

	printf("Initializing Hardware, Stage 1\n");

	DriverManager driverManager;

	PrintMouseEventHandler printMouseEventHandler;
	MouseDriver mouseDriver(&interrupts, &printMouseEventHandler);
	PrintKeyboardEventHandler printKeyboardDriverHandler;
	KeyboardDriver keyboardDriver(&interrupts, &printKeyboardDriverHandler);

	driverManager.AddDriver(&mouseDriver);
	driverManager.AddDriver(&keyboardDriver);

	printf("Initializing Hardware, Stage 2\n");
	driverManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");
	interrupts.Activate();

	while (1)
		;
}
