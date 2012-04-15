#ifndef __SCH__
#define __SCH__
#include "../include/kasm.h"
#include "../include/libc.h"

#define MAXPROC 10
#define STACK_SIZE 64
typedef enum {RUN, BLOCK, READY} status_t;

typedef struct
{
	status_t status;
	int pid;
	int priority;
	int ss, ssize;
	int sp;
} task_t;


typedef struct
{
	int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX,  EIP, CS, EFLAGS;
	void* retaddr;
	int argc;
	char** argv;
} stackframe_t;

int initStackFrame(int (*funct)(int, char **), int argc, char** argv, int bot, void(*clean)());
void saveStack(int ESP);
void* getIdleStack(void);
task_t* getNextProcess (void);
task_t* getNextTask (void);
int getStack(task_t* proc);
void initScheduler();
void cleaner(void);
int idle(int argc, char* argv[]);

#endif
