#ifndef __STDIO__
#define __STDIO__

#include "systemcalls.h"
#include "stdarg.h"
#include "string.h"

void printf(char * format, ...);
void putchar(char c);
void speak(char*c);
void putc(int f, char c);
void set_out_stream(int i);
char getchar();
char* gets(char* buffer);
int scanf(const char* format, ...);

#endif
