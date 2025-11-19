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
#include <hardwarecommunication/pci.hpp>
#include <gui/desktop.hpp>
#include <gui/window.hpp>

#define GRAPHICS_MODE true;

extern "C" void
kernel_main(void)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel\n");

	GlobalDescriptorTable gdt;
	InterruptManager interruptManager(0x20, &gdt);

	printf("Initializing Hardware, Stage 1\n");

#ifdef GRAPHICS_MODE
	Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif

	DriverManager driverManager;

#ifdef GRAPHICS_MODE
	MouseDriver mouseDriver(&interruptManager, &desktop);
#else
	PrintMouseEventHandler printMouseEventHandler;
	MouseDriver mouseDriver(&interruptManager, &printMouseEventHandler);
#endif

#ifdef GRAPHICS_MODE
	KeyboardDriver keyboardDriver(&interruptManager, &desktop);
#else
	PrintKeyboardEventHandler printKeyboardDriverHandler;
	KeyboardDriver keyboardDriver(&interruptManager, &printKeyboardDriverHandler);
#endif

	driverManager.AddDriver(&mouseDriver);
	driverManager.AddDriver(&keyboardDriver);

	PeripheralComponentInterconnectController PCIController;
	PCIController.SelectDrivers(&driverManager, &interruptManager);

	VideoGrapicsArray vga;

	printf("Initializing Hardware, Stage 2\n");
	driverManager.ActivateAll();

	printf("Initializing Hardware, Stage 3\n");

#ifdef GRAPHICS_MODE
	vga.SetMode(320, 200, 8);

	Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
	desktop.AddChild(&win1);
	Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
	desktop.AddChild(&win2);
#endif

	interruptManager.Activate();

	while (1)
	{
#ifdef GRAPHICS_MODE
		desktop.Draw(&vga);
		vga.RenderScreen();
#endif
	};
}
