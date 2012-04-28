#ifndef __PAGING__
#define __PAGING__

#define MEM_START 0x00200000
#define PAGE_SIZE 4096
#define KERNEL_PAGES 530
#define USER_PAGES 1024-KERNEL_PAGES

#include "kasm.h"
#include "int80.h"
#include "stdio.h"
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
void freeProcesPages(int pid);

#endif
