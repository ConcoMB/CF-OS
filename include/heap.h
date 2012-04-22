#include "scheduler.h"

typedef struct 
{
	int front, back, free;
}head_t;

void * memoAlloc(int size);

head_t* advance(head_t* head);

head_t* backward(head_t* head);

void free(void* dir);
