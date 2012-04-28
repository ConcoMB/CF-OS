#ifndef __STRUCTS__
#define __STRUCTS__
#define MAXPROC 10


typedef enum {RUN, BLOCK, READY, FREE} status_t;

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
	char * name;
	status_t status;
	int ticks;
	int timeBlocks;
	int pid;
	int parentPid;
	int priority;
	int ss, ssize, heap;
	stackframe_t* sp;
	tty_t *tty;
	int ttyN;
} task_t;

typedef struct 
{
	char * names[MAXPROC];
	int pids[MAXPROC];
	int percent[MAXPROC];
	int cant;
}topInfo_t;

#endif
