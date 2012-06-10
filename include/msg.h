#ifndef __MSG__

#define __MSG__

#define MAXMSG 20

typedef struct
{
	int comand;
	int argc;
	void** argv;
} msg_t;

extern msg_t msgs[];
extern int readIndex;
extern int msgCount;
extern int writeIndex;
extern int writeBlock;
extern int readBlock;

#endif