//****************************************************************************
//**
//**    pit.cpp
//**		8253 Programmable Interval Timer
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "pit.h"
#include <idt.h>
#include <hal.h>

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

//-----------------------------------------------
//	Controller Registers
//-----------------------------------------------

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43

//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

//! Global Tick count
//static volatile uint32_t			_pit_ticks=0;

//! Test if pit is initialized
static bool							_pit_bIsInit=false;
volatile uint32_t g_pitTicks;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

//! pit timer interrupt handler
//void _cdecl i86_pit_irq(registers_t);

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

//!	pit timer interrupt handler
/*void _cdecl i86_pit_irq(registers_t regs) {

	//__asm__ __volatile__("add $12, %%esp" : );
	//__asm__("pusha");

	//! increment tick count
	_pit_ticks++;

	//! tell hal we are done
	//interruptdone(0);

	//__asm__ ("popa");
	//__asm__("iret");
}*/

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! Sets new pit tick count and returns prev. value
uint32_t i86_pit_set_tick_count (uint32_t i) {

	uint32_t ret = g_pitTicks;
	g_pitTicks = i;
	return ret;
}


//! returns current tick count
uint32_t i86_pit_get_tick_count () {

	return g_pitTicks;
}


//! send command to pit
void i86_pit_send_command (uint8_t cmd) {

	outb (I86_PIT_REG_COMMAND, cmd);
}


//! send data to a counter
void i86_pit_send_data (uint16_t data, uint8_t counter) {

	uint8_t	port= (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	outb(port, (uint8_t)data);
}


//! read data from counter
uint8_t i86_pit_read_data (uint16_t counter) {

	uint8_t	port= (counter==I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter==I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	return inb (port);
}


//! starts a counter
void i86_pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode) {

	if (freq==0)
		return;

	uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

	//! send operational command
	uint8_t ocw=0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
	i86_pit_send_command (ocw);

	//! set frequency rate
	i86_pit_send_data (divisor & 0xff, 0);
	i86_pit_send_data ((divisor >> 8) & 0xff, 0);

	//! reset tick count
	g_pitTicks = 0;
	_pit_bIsInit = true;
}



//! test if pit interface is initialized
bool _cdecl i86_pit_is_initialized () {

	return _pit_bIsInit;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[pit.cpp]
//**
//****************************************************************************
