#ifndef __STRUCTS__
#define __STRUCTS__

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
	status_t status;
	int timeBlocks;
	int pid;
	int priority;
	int ss, ssize, heap;
	stackframe_t* sp;
	tty_t *tty;
} task_t;

#endif
