#include "intr.h"
#include <idt.h>
#include <pic.h>
#include <local_apic.h>
#include <ioapic.h>
#include <stddef.h>

// ------------------------------------------------------------------------------------------------
extern void pit_interrupt();
extern void spurious_interrupt();

// ------------------------------------------------------------------------------------------------
void IntrInit()
{
	// Build Interrupt Table
	IdtInit();
	IdtSetHandler(INT_TIMER, INTERRUPT_GATE, pit_interrupt);
	IdtSetHandler(IRQ(0), INTERRUPT_GATE, NULL);
	IdtSetHandler(INT_SPURIOUS, INTERRUPT_GATE, spurious_interrupt);

	// Initialize subsystems
	PicInit();
	LocalApicInit();
	IoApicInit();
	
}