#ifndef __VID__
#define __VID__
#include "kernel.h"
#include "scheduler.h"

#define VID_DIR 0xb8000
#define WIDTH 80
#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro
#define BLACK_TXT 0x70

typedef struct
{
	char * video;
	char color;
	int cursor_x;
	int cursor_y;
} tty_t;


void initScreens();
void k_clear_screen();
void sys_print(char c);
void sys_setcolor(char c);
void scroll();
int cursor_index();
void update_cursor(void);
void swapTTY(int num);

#endif
