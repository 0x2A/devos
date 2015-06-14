#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	extern void* memcpy(void *dest, const void *src, size_t count);
	extern void *memset(void *dest, int val, size_t count);
	extern void *memsetw(void *dest, int val, size_t count);


	extern int strcmp(const char* str1, const char* str2);
	extern char* strchr(char * str, int character);
	extern size_t strlen(const char *str);
	extern char *strcpy(char *s1, const char *s2);

#ifdef __cplusplus
}
#endif