#ifndef _systemcalls_
#define _systemcalls_


#include "kasm.h"
#include "video.h"
#include "defs.h"
#include "paging.h"
#include "speaker.h"
#include "kernel.h"
//#include "kb.h"
#include "heap.h"



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
int sys_heap_count();
int sys_stack_count(unsigned int esp);
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,byte cero);


#endif
