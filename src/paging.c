#define MEM_START 0x00200000
#define PAGE_SIZE 4096
#define KERNEL_PAGES 530
#define USER_PAGES 1024-KERNEL_PAGES

#include "../include/kasm.h"
#include "../include/int80.h"

int* dir = (void*)MEM_START;
int** page_table = (void*)MEM_START + PAGE_SIZE;
char page_present[USER_PAGES];
int current_page_num=1;

void page_init(){
	int i;
	for(i=0; i<USER_PAGES; i++){
		page_present[i]=0;
	}
	_lcr3(dir);
	*dir=(int)page_table;
	(*dir)=(*dir)|0x00000001;
	_fill_page1();
	_epag();
}

void* getPage()
{
	int i;
	for(i=0; i<USER_PAGES; i++)
	{
		if(!page_present[i])
		{
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	return 0;
}

void freePage(void* address)
{
	int i=(int)address/PAGE_SIZE - KERNEL_PAGES;
	page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])&0xFFFFFFFE);
}


void * sys_calloc(int bytes){
	return 0;
}

int sys_heap_count(){
	int count=0;
	int i;
	for(i=0; i<USER_PAGES; i++){
		if(page_present[i]){
			count++;
		}
	}
	return count;
}

void page_fault(int fault){
	char* s="\npage fault";
	int i;
	for(i=0;i<11;i++){
		sys_print(s[i]);
	}
	while(1);
}

