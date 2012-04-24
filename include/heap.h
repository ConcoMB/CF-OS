#ifndef __HEAP__
#define __HEAP__
#include "scheduler.h"
#include "paging.h"

typedef struct 
{
	int front, back, free;
}head_t;

void * sys_malloc(int size);

head_t* advance(head_t* head);

head_t* backward(head_t* head);

int sys_free(void* dir);

void initHeap(void* dir);

head_t* controlRealloc(head_t* heap, int size);
#endif
