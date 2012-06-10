#ifndef __MSG__

#define __MSG__

#include "../include/fileCalls.h"

#define MAXMSG 20


typedef struct
{
	int pid;
	int command;
	int argc;
	void * argv[4];
} msg_t;

extern msg_t msgs[];
extern int msgTail;
extern int msgCount;
extern int msgHead;

int msgRead(msg_t * ans);

int msgWrite(msg_t * toWrite);


#endif