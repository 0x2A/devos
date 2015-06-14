#include "tty.h"
#include <terminal.h>
#include <keyboard.h>
#include <system.h>
#include <string.h>


void tty_promt() 
{
	//TODO: insert VFS letter and user in here
	puts("\nkrnl:$> ");
}


//! wait for key stroke
KEYCODE	getch() {

	KEYCODE key = KEY_UNKNOWN;

	//! wait for a keypress
	while (key == KEY_UNKNOWN)
		key = kkybrd_get_last_key();

	//! discard last keypress (we handled it) and return
	kkybrd_discard_last_key();
	return key;
}


//! gets next command
void tty_get_cmd(char* buf, int n) {

	KEYCODE key = KEY_UNKNOWN;
	bool	BufChar;

	//! get command string
	int i = 0;
	while (i < n) {

		//! buffer the next char
		BufChar = true;

		//! grab next char
		key = getch();

		//! end of command if enter is pressed
		if (key == KEY_RETURN)
			break;

		//! backspace
		if (key == KEY_BACKSPACE) {

			//! dont buffer this char
			BufChar = false;

			if (i > 0) {

				//! go back one char
				unsigned y, x;
				TermGetcursorPos(&x, &y);
				if (x > 0)
					TermSetCursorPos(--x, y);
				else {
					//! x is already 0, so go back one line
					y--;
					x = VGA_WIDTH;
				}

				//! erase the character from display
				putch(' ');
				TermSetCursorPos(x, y);

				//! go back one char in cmd buf
				i--;
			}
		}

		//! only add the char if it is to be buffered
		if (BufChar) {

			//! convert key to an ascii char and put it in buffer
			char c = kkybrd_key_to_ascii(key);
			if (c != 0) { //insure its an ascii char

				putch(c);
				buf[i++] = c;
			}
		}

		//! wait for next key. You may need to adjust this to suite your needs
		sleep(10);
	}

	//! null terminate the string
	buf[i] = '\0';
}

//! read command
void cmd_read() {

	//! get pathname
	char path[32];
	
	tty_get_cmd(path, 32);

	//! open file
	/*FILE file = volOpenFile(path);

	//! test for invalid file
	if (file.flags == FS_INVALID) {

		DebugPrintf("\n\rUnable to open file");
		return;
	}

	//! cant display directories
	if ((file.flags & FS_DIRECTORY) == FS_DIRECTORY) {

		DebugPrintf("\n\rUnable to display contents of directory.");
		return;
	}

	//! top line
	DebugPrintf("\n\n\r-------[%s]-------\n\r", file.name);

	//! display file contents
	while (file.eof != 1) {

		//! read cluster
		unsigned char buf[512];
		volReadFile(&file, buf, 512);

		//! display file
		for (int i = 0; i < 512; i++)
			DebugPutc(buf[i]);

		//! wait for input to continue if not EOF
		if (file.eof != 1) {
			DebugPrintf("\n\r------[Press a key to continue]------");
			getch();
			DebugPrintf("\r"); //clear last line
		}
	}

	//! done :)
	DebugPrintf("\n\n\r--------[EOF]--------");*/
}

//! our simple command parser
bool tty_run_cmd(char* cmd_buf) {

	if (cmd_buf[0] == '\0')
		return false;

	//! exit command
	if (strcmp(cmd_buf, "exit") == 0) {
		cls();
		puts("system halt.");
		return true;
	}

	//! clear screen
	else if (strcmp(cmd_buf, "cls") == 0) {
		cls();
	}

	else if (strcmp(cmd_buf, "ls") == 0 || strcmp(cmd_buf, "dir") == 0){
		puts("\nfailed: no filsystem implemented yet.");
	}

	//! help
	else if (strcmp(cmd_buf, "help") == 0) {

		puts("\nOS Development Series VFS Programming Demo");
		puts("Supported commands:\n");
		puts(" - exit: quits and halts the system\n");
		puts(" - cls: clears the display\n");
		puts(" - help: displays this message\n");
		puts(" - read: reads a file\n");
		puts(" - reset: Resets and recalibrates floppy for reading\n");
	}

	//! read sector
	else if (strcmp(cmd_buf, "read") == 0) {
		cmd_read();
	}

	//! invalid command
	else {
		puts("\nUnkown command");
	}

	return false;
}

void tty_run() {

	//! command buffer
	char	cmd_buf[100];

	while (1) {

		//! display prompt
		tty_promt();

		//! get command
		tty_get_cmd(cmd_buf, 98);

		//! run command
		if (tty_run_cmd(cmd_buf) == true)
			break;
	}
}