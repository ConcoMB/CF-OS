#ifndef __KB__
#define __KB__
//#include "kasm.h"
//#include "defs.h"
//#include "kernel.h"
//#include "video.h"
#define BUFFER_SIZE 10
#define ESP 1
#define ENG 2

void kb_init();
void int_09();
char buffer_getchar();
void buffer_putchar(char c);
void sys_set_scancode(int i);
void update_leds(char status);
char to_upper(char c);
char to_lower(char c);

#endif
