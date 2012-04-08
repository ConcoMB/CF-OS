#ifndef __SHELL__
#define __SHELL__

#define QUIT 12345
#include <string.h>
#include <stdio.h>
#include "../msg.h"
#include "../common.h"
#include "../colors.h"
#include <sys/wait.h>

extern void* channel;

void shell(int msgID, char* username);

#endif
