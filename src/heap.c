#include "../include/heap.h"

int CONDITION=sizeof(head_t)+4;


void * sys_malloc(int size)
{
	head_t* heap=(head_t*)process[current].heap;
	while(1)
	{
		if(heap->free)
		{
			int dif=heap->front-size;
			if(heap->front==0)
			{
				heapResize(heap, size);
				heap->free=0;
				heap->front=size;
				heap=advance(heap);
				heap->front=0;
				heap->back=size;
				heap->free=1;
				return backward(heap)+1;
			}
			else if(dif>=0 && dif<CONDITION)
			{
				heap->free=0;
				return heap+1;
			}
			else if(dif>=CONDITION)
			{
				heap->free=0;
				heap->front=size;
				heap=advance(heap);
				heap->front=dif-sizeof(head_t);
				heap->back=size;
				heap->free=1;
				return backward(heap)+1;
			}
		}
		heap=advance(heap);
	}
}

void heapResize(head_t* heap, int size)
{
	int occupied = ((int)heap+sizeof(head_t))%4096;
	if(4096-occupied < size+CONDITION)
	{
		getHeapPage(current);
		return;
	}
}

head_t* advance(head_t* head)
{
	char* dir=(char*)head;
	dir+=head->front+sizeof(head_t);
	return (head_t*)dir;
}


head_t* backward(head_t* head)
{
	char* dir=(char*)head;
	dir-=(head->back+sizeof(head_t));
	return (head_t*)dir;
}

int sys_free(void* dir)
{
	int acum=0, last=0;
	head_t* heap=(head_t*)(dir-sizeof(head_t));
	if(heap->free==1)
	{
		return 0;
	}
	head_t* next=advance(heap);
	if(next->front)
	{
		if(next->free)
		{
			acum+=next->front+sizeof(head_t);
			next=advance(next);
		}
	}
	else
	{
		last=1;
	}
	head_t* prev;
	if(heap->back)
	{
		prev=backward(heap);
		if(prev->free)
		{
			acum+=heap->front+sizeof(head_t);
		}
		else
		{
			prev=heap;
		}
	}
	else
	{
		prev=heap;
	}
	if(!last)
	{
		prev->front+=acum;
	}
	else
	{
		prev->front=0;
	}
	next->back=prev->front;
	prev->free=1;
	return 1;
}

void initHeap(void* dir)
{
	head_t* heap=(head_t*)dir;
	heap->front=0;
	heap->back=0;
	heap->free=1;
}
