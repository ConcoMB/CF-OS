#ifndef __SHELL__
#define __SHELL__

#define MAX_CMD_SIZE 70
#define CMD_MEMORY 5
#define SCREEN 1
#define SPEAKER 4
#include "libc.h"
#include "string.h"
#include "extras.h"
#include "systemcalls.h"
#include "stdio.h"


char color(char* color_name);
void set_out_stream_command(char* c);
void print_memory();
int shell(int argc, char** argv);
void help();

#endif
