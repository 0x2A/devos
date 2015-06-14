#pragma once

#include <stdint.h>

// ------------------------------------------------------------------------------------------------
#define INTERRUPT_GATE              0x8e00
#define TRAP_GATE                   0x8f00

typedef struct registers
{
	uint32_t ds;                  // Data segment selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;


// ------------------------------------------------------------------------------------------------
void IdtInit();
void IdtSetHandler(uint8_t index, uint16_t type, void(*handler)());


// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void(*isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);