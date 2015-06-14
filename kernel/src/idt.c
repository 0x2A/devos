#include "idt.h"
#include <system.h>
#include <stddef.h>

#define IDT_BASE                    0x00001000

// ------------------------------------------------------------------------------------------------
typedef struct IdtDesc
{
	uint16_t limit;
	uint64_t base;
} PACKED IdtDesc;


typedef struct IdtEntry
{
	uint16_t offset0;
	uint16_t selector;
	uint16_t type;
	uint16_t offset1;
	uint32_t offset2;
	uint32_t reserved;
} PACKED IdtEntry;

typedef void(*Handler)();

extern void default_exception_handler();
extern void default_interrupt_handler();
extern void(*exception_handlers[20])();

isr_t exception_handler_entries[255];

// ------------------------------------------------------------------------------------------------
void IdtInit()
{
	// Build initial IDT
	for (uint32_t i = 0; i < 20; ++i)
	{
		IdtSetHandler(i, INTERRUPT_GATE, exception_handlers[i]);
	}

	for (uint32_t i = 20; i < 32; ++i)
	{
		IdtSetHandler(i, INTERRUPT_GATE, default_exception_handler);
	}

	for (uint32_t i = 32; i < 256; ++i)
	{
		IdtSetHandler(i, TRAP_GATE, default_interrupt_handler);
	}

	IdtDesc idtDesc =
	{
		.limit = 256 * sizeof(IdtEntry) - 1,
		.base = IDT_BASE
	};
	__asm__ volatile("lidt %0" : : "m" (idtDesc) : "memory");

	// Test interrupt
	// __asm__ volatile("int $3");
}

// ------------------------------------------------------------------------------------------------
static void IdtSetEntry(uint8_t index, uint64_t base, uint16_t selector, uint16_t type)
{
	IdtEntry *entry = (IdtEntry *)IDT_BASE + index;

	entry->offset0 = (uint16_t)base;
	entry->selector = selector;
	entry->type = type;
	entry->offset1 = (uint16_t)(base >> 16);
	entry->offset2 = (uint32_t)(base >> 32);
	entry->reserved = 0;
}

// ------------------------------------------------------------------------------------------------
void IdtSetHandler(uint8_t index, uint16_t type, void(*handler)())
{
	if (handler)
	{
		uint16_t selector = 0x8; // gdt.code
		IdtSetEntry(index, (uint32_t)handler, selector, type);
	}
	else
	{
		IdtSetEntry(index, 0, 0, 0);
	}
}

// This gets called from our ASM interrupt handler stub.
void except_handler(registers_t regs)
{
	/*puts("recieved interrupt: ");
	write_dec(regs.int_no);
	putch('\n');*/

	if (exception_handler_entries[regs.int_no] != 0)
	{
		isr_t handler = exception_handler_entries[regs.int_no];
		handler(regs);
	}

}

void register_interrupt_handler(uint8_t n, isr_t handler)
{
	exception_handler_entries[n] = handler;
}