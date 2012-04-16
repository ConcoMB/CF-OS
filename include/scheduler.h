#ifndef __SCH__
#define __SCH__
#include "../include/kasm.h"
#include "../include/libc.h"

#define MAXPROC 10
#define STACK_SIZE 128
typedef enum {RUN, BLOCK, READY} status_t;


typedef struct
{
	int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX,  EIP, CS, EFLAGS;
	void* retaddr;
	int argc;
	char** argv;
} stackframe_t;

typedef struct
{
	status_t status;
	int pid;
	int priority;
	int ss, ssize;
	stackframe_t* sp;
} task_t;

task_t* getProcess(int current);
stackframe_t* initStackFrame(int (*funct)(int, char **), int argc, char** argv, int bot, void(*clean)());
void saveStack(stackframe_t* ESP);
void* getIdleStack(void);
task_t* getNextProcess (void);
task_t* getNextTask (void);
stackframe_t* getStack(task_t* proc);
void initScheduler();
void cleaner(void);
int idle(int argc, char* argv[]);

#endif
