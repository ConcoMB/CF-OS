#ifndef _systemcalls_
#define _systemcalls_


#include "kasm.h"
#include "kc.h"
#include "defs.h"
#include "int80.h"
#include "paging.h"
#include "speaker.h"
#include "kernel.h"
#include "kb.h"

#define WIDTH 80

/* Tiempo*/
typedef struct {
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;
} time_t;

void sys_print(char c);
void sys_read(char *c);
void sys_hour(char* hp);
void sys_min(char* mp);
void sys_setcolor(char c);
int sys_heap_count();
int sys_stack_count(unsigned int esp);

#endif
