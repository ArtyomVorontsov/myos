#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.hpp>
#include <kernel/interrupts.hpp>
#include <driver/keyboard.hpp>
#include <driver/mouse.hpp>
#include <driver/driver.hpp>
#include <driver/driver-manager.hpp>

class PrintKeyboardEventHandler : public KeyboardEventHandler
{
public:
	void OnKeyDown(char c)
	{
		char *foo = " ";
		foo[0] = c;
		printf(foo);
	};
};

class PrintMouseEventHandler : public MouseEventHandler
{

	int8_t x, y;

public:
	virtual void OnActivate()
	{

		uint16_t *VideoMemory = (uint16_t *)0xb8000;

		x = 40;
		y = 12;

		// Invert color on startup position
		VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
								  (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
								  (VideoMemory[80 * y + x] & 0x00FF);
	};

	virtual void OnMouseMove(int xOffset, int yOffset)
	{
		// Invert color of previous position
		static uint16_t *VideoMemory = (uint16_t *)0xb8000;
		VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
								  (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
								  (VideoMemory[80 * y + x] & 0x00FF);

		x += xOffset;
		if (x >= 80)
			x = 79;
		if (x < 0)
			x = 0;
		y -= yOffset;
		if (y >= 25)
			y = 24;
		if (y < 0)
			y = 0;

		// Invert color of current position
		VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 |
								  (VideoMemory[80 * y + x] & 0x0F000) >> 4 |
								  (VideoMemory[80 * y + x] & 0x00FF);
	};
};

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
