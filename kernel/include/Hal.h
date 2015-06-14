#pragma once

//****************************************************************************
//**
//**    Hal.h
//**		Hardware Abstraction Layer Interface
//**
//**	The Hardware Abstraction Layer (HAL) provides an abstract interface
//**	to control the basic motherboard hardware devices. This is accomplished
//**	by abstracting hardware dependencies behind this interface.
//**
//**	All routines and types are declared extern and must be defined within
//**	external libraries to define specific hal implimentations.
//**
//****************************************************************************


//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>
#include <stdbool.h>
#include <system.h>
#include <pit.h>
#include <descriptor_tables.h>
#include <intr.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

#ifdef _MSC_VER
#define interrupt __declspec (naked)
#else
#define interrupt
#endif


#define far
#define near


//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

//! initialize hardware abstraction layer
extern	int _cdecl hal_initialize ();

//! shutdown hardware abstraction layer
extern	int				_cdecl	hal_shutdown ();

//! enables hardware device interrupts
extern	void			_cdecl	enableInterrupts ();

//! disables hardware device interrupts
extern	void			_cdecl	disableInterrupts ();

//! generates interrupt
extern	void			_cdecl	geninterrupt (int n);

//! reads from hardware device port
extern	unsigned char	_cdecl	inb (unsigned short id);

//! writes byte to hardware port
extern	void			_cdecl	outb (unsigned short id, unsigned char value);


//! generates sound
extern	void			_cdecl	sound (unsigned frequency);

//! returns cpu vender
extern const char*		_cdecl	get_cpu_vender ();

//! returns current tick count (Only for demo)
extern	int				_cdecl	get_tick_count ();

//! DMA Routines provided for driver use
extern	void dma_set_mode (uint8_t channel, uint8_t mode);
extern	void dma_set_read (uint8_t channel);
extern	void dma_set_write (uint8_t channel);
extern	void dma_set_address(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_set_count(uint8_t channel, uint8_t low, uint8_t high);
extern	void dma_mask_channel (uint8_t channel);
extern	void dma_unmask_channel (uint8_t channel);
extern	void dma_reset_flipflop (int dma);
extern  void dma_enable (uint8_t ctrl, bool e);
extern  void dma_reset (int dma);
extern  void dma_set_external_page_register (uint8_t reg, uint8_t val);
extern  void dma_unmask_all (int dma);

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END [Hal.h]
//**
//****************************************************************************

