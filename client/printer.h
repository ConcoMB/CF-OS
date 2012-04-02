#ifndef __PRINT__
#define __PRINT__

#include <curses.h>
#include <stdlib.h>
#include <stdarg.h>

void alignement();

void printTitle();

void initWindow();

void printCommandLine();

void alinement();

void scrollUp();

void scan(char* fmt, ...);

void printString(char* fmt, ...);

#endif
