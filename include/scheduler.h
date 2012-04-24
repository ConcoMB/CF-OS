#ifndef __SCH__
#define __SCH__
#include "kasm.h"
#include "stdio.h"
#include "paging.h"
#include "video.h"
#include "structs.h"
#include "heap.h"

#define MAXPROC 10
#define MAXPAGEPERPROC 10
#define STACK_SIZE 4096


<<<<<<< HEAD
typedef struct
{
	char * video;
	char color;
	int cursor_x;
	int cursor_y;
} tty_t;

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
	int ss, ssize, heap;
	stackframe_t* sp;
	tty_t *tty;
} task_t;
=======
>>>>>>> 81a0c9e5aee99ef816d9b7d0f0e4e89cd20acbde

extern task_t process[];
extern int current;
extern tty_t terminals[];

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
void createProcess(int (*funct)(int, char **), int p, int ttyN);
int processHasFocus();

#endif
