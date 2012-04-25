#ifndef __VID__
#define __VID__
#include "kernel.h"
#include "scheduler.h"
#include "structs.h"

#define VID_DIR (char*)0xb8000
#define WIDTH 80
#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro
#define BLACK_TXT 0x70

extern int currentTTY;

void initScreens();
void clear_screen(tty_t* tty);
void sys_print(char c);
void sys_setcolor(char c);
void scroll(tty_t* tty);
int cursor_index(tty_t* tty);
void update_cursor(tty_t* tty);
void swapTTY(int num);
void printScancode();
void printBar();

#endif
