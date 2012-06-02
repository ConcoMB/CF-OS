#ifndef __PAGING__
#define __PAGING__

#define MEM_START 0x00200000
#define PAGE_SIZE 4096
#define KERNEL_PAGES 530
#define USER_PAGES 1024-KERNEL_PAGES
#define CANTBLOCKS ((USER_PAGES)/(BLOCKSIZE))
#define ST ((char)1)
#define HP ((char)2)
#include "kasm.h"
#include "int80.h"
#include "stdio.h"
#include "status.h"
#include "scheduler.h"

void * sys_calloc(int bytes);
void page_fault();
void page_init();
int heap_count();
void* getStackPage(int pid);
void* getHeapPage(int pid);
//void heapRealloc(int pid);
//void stackRealloc(int pid);
//void* pageRealloc(void* heap, int pCant, int diff);
void freePage(void* address);
void freeProcessPages(int pid);
void setBlock(int pid);
void initBlocks();
int getInitBlock(int pid);
int getCantBlocks(int init, int pid);
int getFreeSpace(int cant);
void moveStack(int pid, int from, int to, int cant);
void moveHeap(int pid, int from, int to, int cant);
void reallocMe(int pid, char which);


#endif
