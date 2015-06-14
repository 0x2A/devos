#pragma once

#include <system.h>
#include <stdint.h>
#include <vga.h>
#include <stdarg.h>

void cls();
void putch(char c);
void puts(const char *str);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void terminal_initialize();
void write_dec(uint32_t n);
void write_hex(uint32_t n);

extern void TermGetcursorPos(unsigned* x, unsigned* y);
extern void TermSetCursorPos(unsigned x, unsigned y);

extern int vDbgPrintf(const char* str, va_list args);
int DbgPrintf(const char* str, ...);