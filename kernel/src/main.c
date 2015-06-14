#include <system.h>
#include <vga.h>
#include <terminal.h>
#include <hal.h>
#include <idt.h>
#include <acpi.h>
#include <paging.h>
#include <kheap.h>
#include <exception.h>
#include <keyboard.h>
#include <tty.h>

extern void panic(const char *message, const char *file, uint32_t line)
{
	// We encountered a massive problem and have to stop.
	asm volatile("cli"); // Disable interrupts.

	puts("KERNEL PANIC: ");
	puts(message);
	puts(" at ");
	puts(file);
	puts(":");
	write_dec(line);
	puts("\n");

	// Halt by going into an infinite loop.
	for (;;);
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
	// An assertion failed, and we have to panic.
	asm volatile("cli"); // Disable interrupts.

	puts("ASSERTION-FAILED: ");
	puts(desc);
	puts(" at ");
	puts(file);
	puts(":");
	write_dec(line);
	puts("\n");

	// Halt by going into an infinite loop.
	for (;;);
}


void init()
{
	// Initialise global descriptor table

	puts("Init GDT and paging\n");
	init_descriptor_tables();
	initialise_paging();

	puts("Init ACPI\n");
	//AcpiInit();

	hal_initialize();

	//
	register_interrupt_handler(0, divide_by_zero_fault);
	register_interrupt_handler(1, single_step_trap);
	register_interrupt_handler(2, nmi_trap);
	register_interrupt_handler(3, breakpoint_trap);
	register_interrupt_handler(4, overflow_trap);
	register_interrupt_handler(5, bounds_check_fault);
	register_interrupt_handler(6, invalid_opcode_fault);
	register_interrupt_handler(7, no_device_fault);
	register_interrupt_handler(8, double_fault_abort);
	register_interrupt_handler(10, invalid_tss_fault);
	register_interrupt_handler(11, no_segment_fault);
	register_interrupt_handler(12, stack_fault);
	register_interrupt_handler(13, general_protection_fault);
	register_interrupt_handler(16, fpu_fault);
	register_interrupt_handler(17, alignment_check_fault);
	register_interrupt_handler(18, machine_check_abort);
	register_interrupt_handler(19, simd_fpu_fault);

	//! install the keyboard to IR 33, uses IRQ 1

	puts("install keyboard");
	kkybrd_install(IRQ(1));
}

//! sleeps a little bit. This uses the HALs get_tick_count() which in turn uses the PIT
void sleep(int ms) {

	int ticks = ms + get_tick_count();
	while (ticks > get_tick_count())
		;
}

/* This is a very simple main() function. All it does is sit in an
*  infinite loop. This will be like our 'idle' loop */
int kernel_main()
{
	terminal_initialize();
	
	settextcolor(COLOR_LIGHT_BLUE,COLOR_BLACK);
	puts("devOS kernel v0.1 alpha, Jun 2015\n");
	puts("copyright (c) C0dR\n\n");
	settextcolor(COLOR_WHITE,COLOR_BLACK);
	puts("loading system...\n");

	init();

	/*
	puts("testing heap and kalloc:\n\n");

	uint32_t a = kmalloc(8);
	uint32_t b = kmalloc(8);
	uint32_t c = kmalloc(8);
	puts("a: ");
	write_hex(a);
	puts(", b: ");
	write_hex(b);
	puts("\nc: ");
	write_hex(c);

	kfree(c);
	kfree(b);
	uint32_t d = kmalloc(12);
	puts(", d: ");
	write_hex(d);*/

	tty_run();	

	return 0;
}