#include <hal.h>
#include <terminal.h>
#include <stdarg.h>

static char* sickpc = " \
                               _______      \n\
                               |.-----.|    \n\
                               ||x . x||    \n\
                               ||_.-._||    \n\
                               `--)-(--`    \n\
                              __[=== o]___  \n\
                             |:::::::::::|\\ \n\
                             `-=========-`()\n\
                                 DEVOS\n\n";

//! something is wrong--bail out
void _cdecl kernel_panic(const char* fmt, ...) {

	disableInterrupts();

	va_list		args;
	
	//char buf[1024];
	va_start(args, fmt);

	char* disclamer = "DevOS has encountered a problem and has been shut down to prevent\n\
damage to your computer. Any unsaved work might be lost.\n\n\
Please report the following information and restart your computer.\n\
The system has been halted.\n\n";

	cls();
	settextcolor(COLOR_LIGHT_RED, COLOR_BLACK);
	puts(sickpc);
	puts(disclamer);
		
	puts("*** STOP: ");
	vDbgPrintf(fmt, args);

	va_end(args);
	for (;;);
}
