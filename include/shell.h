#ifndef __SHELL__
#define __SHELL__


#include "commands.h"


#define MAX_CMD_SIZE 70
#define CMD_MEMORY 5

char color(char* color_name);
void print_memory();
int shell(int argc, char** argv);

#endif
