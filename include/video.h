#ifndef __VID__
#define __VID__
#include "scheduler.h"

#define VID_DIR (char*) 0xb8000

typedef struct
{
	char* video;
	char color=0x07;
	int cursor_x=0;
	int cursor_y=0;
}tty_t;


void initScreens();
void k_clear_screen();
void sys_print(char c);
void sys_setcolor(char c);
void scroll();
int cursor_index();
void update_cursor(void);
void swapTTY(int num);

#endif