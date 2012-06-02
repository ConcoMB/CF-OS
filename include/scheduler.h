#ifndef __SCH__
#define __SCH__
#include "kasm.h"
#include "stdio.h"
#include "paging.h"
#include "video.h"
#include "structs.h"
#include "status.h"
#include "heap.h"

#define BLOCKSIZE 10
#define STACK_SIZE 4096

#define TICK_FREQUENCY 55


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
void createProcess(int (*funct)(int, char **), int argc, char** argv, char* name, int p, int ttyN, int parid);
void createChild(int (*funct)(int, char **), int argc, char ** argv);
int processHasFocus();
void sys_top();
void stackResize(task_t* task);
task_t* getCurrentProcess();
int sys_kill(int pID);
void sortTop(topInfo_t * topInfo);
void swapTop(topInfo_t * topInfo, int j);



#endif
