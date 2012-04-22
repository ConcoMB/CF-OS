#ifndef __PAGING__
#define __PAGING__

void * sys_calloc(int bytes);
void page_fault();
void page_init();
int heap_count();
void* getPage();
void freePage(void* address);

#endif
