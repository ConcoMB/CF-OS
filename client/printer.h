#ifndef __PRINT__
#define __PRINT__

#include <curses.h>
#include <stdlib.h>


void printTitle();

void initWindow();

void printCommandLine();

void alinement();

void scrollUp();

void scanInt(int *i);

void scanString(char * s);

void printString(char* string);

#endif