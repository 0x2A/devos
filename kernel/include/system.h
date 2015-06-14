#pragma once

//#include <stddef.h> //used for size_t and NULL
#include <stdint.h> //standard filetypes (uint8_t etc)


#ifndef _MSC_VER
#define _cdecl __attribute__((cdecl))
#define PACKED __attribute__((__packed__))
#endif


extern void sleep(int ms);

extern void _cdecl kernel_panic(const char* fmt, ...);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);


/* screen.c */
