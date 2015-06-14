
//****************************************************************************
//**
//**    Hal.cpp
//**		Hardware Abstraction Layer for i86 architecture
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <Hal.h>
#include <cpu.h>
#include <descriptor_tables.h>
#include <pit.h>
#include <intr.h>
#include <acpi.h>
#include <ioapic.h>

//! initialize hardware devices
int _cdecl hal_initialize () {

	//! disable hardware interrupts
	disableInterrupts ();
	
	//initialize iterrupt system
	IntrInit();

	//enable timer
	i86_pit_start_counter (1000, I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);

	// Enable IO APIC entries
	IoApicSetEntry(g_ioApicAddr, AcpiRemapIrq(IRQ_TIMER), INT_TIMER);

	//! enable interrupts
	enableInterrupts ();

	return 0;
}

//! shutdown hardware devices
int _cdecl hal_shutdown () {

	//! shutdown system resources
	i86_cpu_shutdown ();
	return 0;
}


//! generate i86 interrupt request
void _cdecl geninterrupt (int n) {

	__asm__(
		".intel_syntax noprefix;"
		"mov al,byte ptr [%0];"
		"mov byte ptr [genint+1], al;"
		"jmp genint;"
		"genint:\n"
		"int 0;"	// above code modifies the 0 to int number to generate
		: : "r" (n) );
	
	__asm__(".att_syntax noprefix");
}


//! output sound to speaker
void _cdecl	sound (unsigned frequency) {

	//! sets frequency for speaker. frequency of 0 disables speaker
	outb (0x61, 3 | (unsigned char)(frequency<<2) );
}


//! read byte from device using port mapped io
unsigned char _cdecl inb (unsigned short portid) {
	unsigned char rv;
	__asm__ __volatile__("inb %1, %0" : "=a" (rv) : "dN" (portid));
	return rv;
}


//! write byte to device through port mapped io
void _cdecl outb (unsigned short portid, unsigned char value) {
	__asm__ __volatile__("outb %1, %0" : : "dN" (portid), "a" (value));
}


//! enable all hardware interrupts
void _cdecl enableInterrupts () {
	asm volatile("sti");
}


//! disable all hardware interrupts
void _cdecl disableInterrupts () {
	asm volatile("cli");
}





//! returns cpu vender
const char* _cdecl get_cpu_vender () {

	return i86_cpu_get_vender();
}


//! returns current tick count (only for demo)
int _cdecl get_tick_count () {

	return i86_pit_get_tick_count();
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[Hal.cpp]
//**
//****************************************************************************
