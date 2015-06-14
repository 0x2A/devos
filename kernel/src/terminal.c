#include "terminal.h"
#include <system.h>
#include <vga.h>
#include <string.h>
#include <hal.h>

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
uint16_t *terminal_buffer;
uint8_t terminal_color;
int csr_x = 0, csr_y = 0;


/* Scrolls the screen */
void scroll(void)
{
    unsigned blank;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = make_vgaentry(0x20, terminal_color);

    /* Row 25 is the end, this means we need to scroll up */
    if(csr_y >= VGA_HEIGHT)
    {
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0 * VGA_WIDTH; i < 24 * VGA_WIDTH; i++)
		{
			terminal_buffer[i] = terminal_buffer[i + 80];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for (i = 24 * VGA_WIDTH; i < 25 * VGA_WIDTH; i++)
		{
			terminal_buffer[i] = blank;
		}
		// The cursor should now be on the last line.
		csr_y = 24;

    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * VGA_WIDTH + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = make_vgaentry(' ', terminal_color);

    /* Sets the entire screen to spaces in our current
    *  color */
    /*for(i = 0; i < VGA_WIDTH; i++)
        memsetw (terminal_buffer + i * VGA_WIDTH, blank, VGA_WIDTH);*/

	for (int i = 0; i < 80 * 25; i++)
		terminal_buffer[i] = blank;

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Puts a single character on the screen */
void putch(char c)
{
    unsigned short *where;
    unsigned att = terminal_color << 8;

    /* Handle a backspace, by moving the cursor back one space */
    switch(c)
    {
	case 0x08:
        if(csr_x != 0) csr_x--;
    break;
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    case 0x09:
        csr_x = (csr_x + 8) & ~(8 - 1);
    break;
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    case '\r':
        csr_x = 0;
    break;
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    case '\n':
        csr_x = 0;
        csr_y++;
    break;
	}
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    if(c >= ' ')
    {
        where = terminal_buffer + (csr_y * VGA_WIDTH + csr_x);
        *where = c | att;	/* Character AND terminal_colorutes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= VGA_WIDTH)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(const char *text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    terminal_color = make_color(forecolor, backcolor);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void terminal_initialize(void)
{
    terminal_buffer = VGA_MEMORY;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    cls();
}

void write_dec(uint32_t n)
{
	if (n == 0)
	{
		putch('0');
		return;
	}

	int32_t acc = n;
	char c[32];
	int i = 0;
	while (acc > 0)
	{
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;
	int j = 0;
	while (i >= 0)
	{
		c2[i--] = c[j++];
	}
	puts(c2);
}

void write_hex(uint32_t n)
{
	int32_t tmp;

	puts("0x");

	char noZeroes = 1;

	int i;
	for (i = 28; i > 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0)
		{
			continue;
		}

		if (tmp >= 0xA)
		{
			noZeroes = 0;
			putch(tmp - 0xA + 'a');
		}
		else
		{
			noZeroes = 0;
			putch(tmp + '0');
		}
	}

	tmp = n & 0xF;
	if (tmp >= 0xA)
	{
		putch(tmp - 0xA + 'a');
	}
	else
	{
		putch(tmp + '0');
	}

}

char tbuf[64];
char bchars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void itoa(unsigned i, unsigned base, char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top = pos--;
	for (opos = 0; opos < top; pos--, opos++) {
		buf[opos] = tbuf[pos];
	}
	buf[opos] = 0;
}

void ltoa(uint64_t i, unsigned base, char* buf) {
	int64_t pos = 0;
	int64_t opos = 0;
	int64_t top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top = pos--;
	for (opos = 0; opos < top; pos--, opos++) {
		buf[opos] = tbuf[pos];
	}
	buf[opos] = 0;
}

void itoa_s(int i, unsigned base, char* buf) {
	if (base > 16) return;
	if (i < 0) {
		*buf++ = '-';
		i *= -1;
	}
	itoa(i, base, buf);
}

void uitoa_s(unsigned i, unsigned base, char* buf) {
	if (base > 16) return;
	itoa(i, base, buf);
}

void ultoa_s(uint64_t i, unsigned base, char* buf) {
	if (base > 16) return;
	ltoa(i, base, buf);
}

int vDbgPrintf(const char* str, va_list args)
{
	if (!str)
		return 0;
	
	size_t i;
	for (i = 0; i < strlen(str); i++) {
		switch (str[i]) {

		case '%':

			switch (str[i + 1]) {

				/*** characters ***/
			case 'c': {
				char c = va_arg(args, char);
				putch(c);
				i++;		// go to next character
				break;
			}

					  /*** address of ***/
			case 's': {
				char* c = va_arg(args, char*);
				puts(c);
				i++;		// go to next character
				break;
			}

					  /*** integers ***/
			case 'd':
			case 'i': {
				int c = va_arg(args, int);
				char str[32] = { 0 };
				itoa_s(c, 10, str);
				puts(str);
				i++;		// go to next character
				break;
			}

					  /*** display in hex ***/
			case 'X':
			case 'x': 
			{
				
				if (str[i + 2] == '.')
				{
					uint32_t c;
					uint64_t cl;
					char str[64] = { 0 };;
					switch (str[i + 3])
					{
					default:
					case '1':
						c = va_arg(args, uint32_t);
						uitoa_s(c, 16, str);
						puts(str);
						break;
					case '2':
						cl = va_arg(args, uint64_t);
						ultoa_s(cl, 16, str);
						puts(str);
						break;
					}
					i += 2;
				}
				else
				{
					uint32_t c = va_arg(args, uint32_t);
					char str[32] = { 0 };
					uitoa_s(c, 16, str);
					puts(str);
				}

				i++;		// go to next character
				break;
			}

			default:
				//va_end(args);
				return 1;
			}

			break;

		default:
			putch(str[i]);
			break;
		}

	}
	
	return i;
}

int DbgPrintf(const char* str, ...) 
{
	va_list		args;
	va_start(args, str);
	
	vDbgPrintf(str, args);

	va_end(args);
}

void TermGetcursorPos(unsigned* x, unsigned* y)
{
	if (x == 0 || y == 0)
		return;

	*x = csr_x;
	*y = csr_y;
}

void TermSetCursorPos(unsigned x, unsigned y)
{
	csr_x = x;
	csr_y = y;

	move_csr();
}