#include "../include/paging.h"

int* dir = (void*)MEM_START;
int** page_table = (void*)MEM_START + PAGE_SIZE;
char page_present[USER_PAGES];
int current_page_num=1;

void page_init()
{
	int i;
	for(i=0; i<USER_PAGES; i++)
	{
		page_present[i]=0;
	}
	_lcr3(dir);
	*dir=(int)page_table;
	(*dir)=(*dir)|0x00000001;
	_fill_page1();
	_epag();
}

void* getHeapPage(int pid)
{
	int heap=1+(pid*MAXPAGEPERPROC), i;
	for(i=heap; i<heap+MAXPAGEPERPROC; i++)
	{
		if(!page_present[i])
		{	
			printf("heap page given %d\n", i);			
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	sys_kill(pid);
	return 0;
}

void* getStackPage(int pid)
{
	int stack=pid*MAXPAGEPERPROC+MAXPAGEPERPROC, i;
	for(i=stack; i>stack-MAXPAGEPERPROC; i--)
	{
		if(!page_present[i])
		{
			printf("stack page given %d\n", i);
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[1+i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	sys_kill(pid);
	return 0;
}

void freeProcessPages(int pid)
{
	int i, page = 1+(pid*MAXPAGEPERPROC);
	for(i=page ; i<(page+MAXPAGEPERPROC) ; i++)
	{
		if(page_present[i])
		{
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])&0xFFFFFFFE);		
			page_present[i]=0;			
		}
	}
}


void page_fault(int fault)
{
	char* s="\npage fault";
	int i;
	for(i=0;i<11;i++)
	{
		sys_print(s[i]);
	}
	kill(current);
}
