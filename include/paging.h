#ifndef __PAGING__
#define __PAGING__

void * sys_malloc(int bytes);
void * sys_calloc(int bytes);
int sys_free(void* i);
void page_fault();
void page_init();
int heap_count();
void* getPage();
void freePage(void* address);

#endif