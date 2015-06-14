//****************************************************************************
//**
//**    cpu.cpp
//**
//**	This is the processor interface. Everything outside of this module
//**	must use this interface when working on processor data.
//**
//**	A processor is a module that manages the very basic data structures
//**	and data within the system. The processor interface provides the interface
//**	for managing processors, processor cores, accessing processor structures,
//**	and more
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "cpu.h"
//#include "gdt.h"
//#include "idt.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
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
//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! Initializes cpu resources
int i86_cpu_initialize () {

	//! initialize processor tables
	//i86_gdt_initialize ();
	//i86_idt_initialize (0x8);

	return 0;
}

//! shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {


}

//! returns vender name of cpu
char* i86_cpu_get_vender () {

	static char	vender[32] = {0};

	__asm__ __volatile__(".intel_syntax noprefix;"
		"mov		eax, 0;"
		"cpuid;"
		"mov		dword ptr[%0], ebx;"
		"mov		dword ptr[%0 + 4], edx;"
		"mov		dword ptr[%0 + 8], ecx;"
		:  : "r"(vender));

	__asm__(".att_syntax noprefix");

	return vender;
}

//! flush all internal and external processor caches
void i86_cpu_flush_caches () {

	__asm__ __volatile__(
		".intel_syntax noprefix;"
		"cli;"
		"invd;"
		"sti;"
		".att_syntax noprefix;"
		);
}

//! same as above but writes the data back into memory first
void i86_cpu_flush_caches_write () {


	__asm__(
		".intel_syntax noprefix;"
		"cli;"
		"wbinvd;"
		"sti;"
		".att_syntax noprefix;"
		);
}

//! flushes TLB entry
void i86_cpu_flush_tlb_entry (uint32_t addr) {

	__asm__(
		".intel_syntax noprefix;"
		"cli;"
		"invlpg	[%0];"
		"sti;"
		".att_syntax noprefix;"
		: : "r"(addr));

}


//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[String.cpp]
//**
//****************************************************************************
