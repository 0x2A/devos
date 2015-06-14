#pragma once

#include <stdbool.h>

extern void tty_run();
extern void tty_get_cmd(char* buf, int n);
extern void tty_promt();
extern bool tty_run_cmd(char* cmd_buf);