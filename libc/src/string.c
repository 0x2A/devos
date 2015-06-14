#include <string.h>


void *memcpy(void *dest, const void *src, size_t count)
{
	const char *sp = src;
	char *dp = dest;
	while(count--)
		*dp++ = *sp++;
	return dest;
}

void *memset(void *dest, int val, size_t count)
{
	char *temp = dest;
	while(count--) 
		*temp++ = val;
	return dest;
}

void *memsetw(void *dest, int val, size_t count)
{
	unsigned short *temp = (unsigned short *)dest;
	for (size_t i; i < count; i++)
		temp[i] = (unsigned short) val;
	return dest;
}


//! compare two strings
int strcmp(const char* str1, const char* str2) {

	int res = 0;
	while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		++str1, ++str2;

	if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;

	return res;
}

//! copies string s2 to s1
char *strcpy(char *s1, const char *s2)
{
	char *s1_p = s1;
	while (*s1++ = *s2++);
	return s1_p;
}

//! returns length of string
size_t strlen(const char* str) {

	size_t	len = 0;
	while (str[len++]);
	return len;
}

//! locates first occurance of character in string
char* strchr(char * str, int character) {

	do {
		if (*str == character)
			return (char*)str;
	} while (*str++);

	return 0;
}