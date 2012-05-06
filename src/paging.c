#include "../include/paging.h"

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

void* getHeapPage(int pid)
{
	/*int i;
	for(i=0; i<USER_PAGES; i++)
	{
		if(!page_present[i])
		{
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	return 0;*/
	int heap=1+(pid*MAXPAGEPERPROC), i;
	for(i=heap; i<heap+MAXPAGEPERPROC; i++)
	{
		if(!page_present[i])
		{	
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
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	sys_kill(pid);
	return 0;
}
/*
void freePage(void* address)
{
	int i=(int)address/PAGE_SIZE - KERNEL_PAGES;
	page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])&0xFFFFFFFE);
}
*/
void freeProcessPages(int pid){
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

/*
void * sys_calloc(int bytes){
	return 0;
}

int sys_heap_count(){
	int count=0;
	int i;
	for(i=0; i<USER_PAGES; i++)
	{
		if(page_present[i]){
			count++;
		}
	}
	return count;
}
*/
void page_fault(int fault){
	char* s="\npage fault";
	int i;
	for(i=0;i<11;i++){
		sys_print(s[i]);
	}
	while(1);
}
/*
void heapRealloc(int pid)
{
	int heap=USER_PAGES+pid*MAXPAGEPERPROC+MAXPAGEPERPROC, i;
	for(i=heap; i>heap-MAXPAGEPERPROC; i--)
	{
		if(!page_present[i])
		{
			printf("realloc\n");
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return;
		}
	}
	//error
	return;
}

void stackRealloc(int pid)
{
	int stack=USER_PAGES+pid*MAXPAGEPERPROC, i;
	for(i=stack; i<stack+MAXPAGEPERPROC; i++)
	{
		if(!page_present[i])
		{
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return;
		}
	}
	//error
	return;

}*/
/*
void* Realloc(void* dir, int pCant, int diff)
{
	printf("REALLOC\n");
	int i, cant=0;
	int page=(int)dir&PAGE_SIZE;
	if(!page_present[page+1])
	{
		page_present[page+1]=1;
		return (void*)((int)dir+diff);
	}
	else
	{
		//ERROR
		return 0;
	}
	for(i=0; i<USER_PAGES; i++)
	{
		if(!page_present[i])
		{
			cant++;
		}
		else
		{
			cant=0;
		}
		if(cant==pCant+1)
		{
			int j;
			for(j=i-cant; cant>=0; j++, cant--)
			{
				page_present[j]=1;
				page_table[j+KERNEL_PAGES]=(int*)((int)(page_table[j+KERNEL_PAGES])|0x00000001);
			}
			printf("MEMCPY REALLOC\n");
			memcpy(page_table[i-cant], dir, PAGE_SIZE*pCant);

			for(j=0; j<pCant-1; j++)
			{
				freePage((void*)((int)dir +j*PAGE_SIZE));
			}
			return (void*)((i-pCant-1+KERNEL_PAGES)*PAGE_SIZE + diff);
		}
	}
	return 0;
}*/
