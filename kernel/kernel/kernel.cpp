#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
#include <driver/keyboard.hpp>
#include <driver/mouse.hpp>
#include <driver/driver.hpp>
#include <driver/vga.hpp>
#include <driver/driver-manager.hpp>
#include <event-handler/print-keyboard-event-handler.hpp>
#include <event-handler/print-mouse-event-handler.hpp>
#include <hardwirecommunication/pci.hpp>

extern "C" void
kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel\n");

	GlobalDescriptorTable gdt;
	InterruptManager interruptManager(0x20, &gdt);

	printf("Initializing Hardware, Stage 1\n");

	DriverManager driverManager;

	PrintMouseEventHandler printMouseEventHandler;
	MouseDriver mouseDriver(&interruptManager, &printMouseEventHandler);
	PrintKeyboardEventHandler printKeyboardDriverHandler;
	KeyboardDriver keyboardDriver(&interruptManager, &printKeyboardDriverHandler);

	driverManager.AddDriver(&mouseDriver);
	driverManager.AddDriver(&keyboardDriver);

	PeripheralComponentInterconnectController PCIController;
	PCIController.SelectDrivers(&driverManager, &interruptManager);

	VideoGrapicsArray vga;

	printf("Initializing Hardware, Stage 2\n");
	driverManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");
	interruptManager.Activate();

	vga.SetMode(320, 200, 8);

	vga.FillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8);

	for (int32_t y = 0; y < 200; y++)
	{
		for (int32_t x = 0; x < 320; x++)
		{
			vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
		}
	}

	while (1)
		;
}
