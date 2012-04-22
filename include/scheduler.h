#ifndef __SCH__
#define __SCH__
#include "kasm.h"
#include "stdio.h"
#include "paging.h"
#include "heap.h"

#define MAXPROC 10
#define STACK_SIZE 4096
typedef enum {RUN, BLOCK, READY, FREE} status_t;

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

extern task_t process[];
extern int current;

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
void createProcess(int (*funct)(int, char **));

#endif
