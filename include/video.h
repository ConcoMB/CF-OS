#ifndef __VID__
#define __VID__
#include "kernel.h"
#include "scheduler.h"

#define VID_DIR (char*)0xb8000
#define WIDTH 80
#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro
#define BLACK_TXT 0x70

void initScreens();
void k_clear_screen(int ttyN);
void sys_print(char c);
void sys_setcolor(char c);
void scroll(int ttyN);
int cursor_index(int ttyN);
void update_cursor(int ttyN);
void swapTTY(int num);

#endif
