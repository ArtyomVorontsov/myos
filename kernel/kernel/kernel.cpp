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
#include <kernel/memory-management.hpp>

// #define GRAPHICS_MODE true;

extern "C" void taskA()
{
	while (1)
	{
		printf("1111111");
	}
}

extern "C" void taskB()
{
	while (1)
		printf("2222222");
}

extern "C" void taskC()
{
	while (1)
		printf("333333");
}

extern "C" void taskD()
{
	while (1)
		printf("444444");
}

extern "C" void
kernel_main(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
	terminal_initialize();
	printf("Hello, MYOS C++ kernel\n");

	GlobalDescriptorTable gdt;

	uint32_t *memupper = (uint32_t *)(((size_t)multiboot_structure) + 8);

	size_t heap = 10 * 1024 * 1024; // 10mb

	// TODO: rework this and get normal heap pointer instead of hardcoded one
	uint32_t addressOfUpperMemoryInKb = (*memupper) * 1024;
	MemoryManager memoryManager(heap, addressOfUpperMemoryInKb - heap - 10 * 1024);

	printf("heap: 0x");
	printfHex((heap >> 24) & 0xFF);
	printfHex((heap >> 16) & 0xFF);
	printfHex((heap >> 8) & 0xFF);
	printfHex(heap & 0xFF);

	void *allocated = memoryManager.malloc(1024);

	printf("\nallocated 0x");
	printfHex(((size_t)allocated >> 24) & 0xFF);
	printfHex(((size_t)allocated >> 16) & 0xFF);
	printfHex(((size_t)allocated >> 8) & 0xFF);
	printfHex((size_t)allocated & 0xFF);

	printf("\n");

	TaskManager taskManager;

	// Task task1(&gdt, taskA);
	// Task task2(&gdt, taskB);
	// Task task3(&gdt, taskC);
	// Task task4(&gdt, taskD);
	// taskManager.addTask(&task1);
	// taskManager.addTask(&task2);
	// taskManager.addTask(&task3);
	// taskManager.addTask(&task4);

	InterruptManager interruptManager(0x20, &gdt, &taskManager);

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
