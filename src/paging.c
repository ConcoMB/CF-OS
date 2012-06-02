#include "../include/paging.h"

int* dir = (void*)MEM_START;
int** page_table = (void*)MEM_START + PAGE_SIZE;
char page_present[USER_PAGES];
int current_page_num=1;
char blocks[CANTBLOCKS];

void initBlocks()
{
	int i;
	for(i=0; i<CANTBLOCKS; i++){
		blocks[i]=-1;
	}
}

void setBlock(int pid)
{
	int i;
	for(i=0; i<CANTBLOCKS; i++){
		if(blocks[i]==-1){
			blocks[i]=pid;
			return;
		}
	}
}

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


int getInitBlock(int pid)
{
	int i=0;
	for(; i<CANTBLOCKS; i++){
		if(blocks[i]==pid){
			return i;
		}
	}
	return -1;
}


void* getHeapPage(int pid)
{
	int heap=1+getInitBlock(pid)*BLOCKSIZE, i;
	for(i=heap; i<heap+BLOCKSIZE; i++)
	{
		if(!page_present[i])
		{	
			printf("heap page given %d\n", i);			
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	//reallocMe(pid, HP);
	kill(pid);
	return 0;
}

void* getStackPage(int pid)
{
	int i=getInitBlock(pid);
	int cant=getCantBlocks(i, pid);
	int stack=getInitBlock(pid)*BLOCKSIZE+BLOCKSIZE*cant;
	for(i=stack; i>stack-BLOCKSIZE; i--)
	{
		if(!page_present[i])
		{
			printf("stack page given %d\n", i);
			page_present[i]=1;
			page_table[i+KERNEL_PAGES]=(int*)((int)(page_table[1+i+KERNEL_PAGES])|0x00000001);
			return (void*)((i+KERNEL_PAGES)*PAGE_SIZE);
		}
	}
	//reallocMe(pid, ST);
	kill(pid);
	return 0;
}

void freeProcessPages(int pid)
{
	int i, page = 1+getInitBlock(pid)*BLOCKSIZE;
	for(i=page ; i<(page+BLOCKSIZE) ; i++)
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

int getCantBlocks(int init, int pid){
	int cant=1;
	for(++init; init<CANTBLOCKS; cant++, init++){
		if(blocks[init]!=pid){
			return cant;
		}
	}
	return cant;
}

int getFreeSpace(int cant){
	int i, sum;
	for(i=0; i<CANTBLOCKS; i++){
		if(blocks[i]==-1){
			char flag=0;
			for(++i, sum=1; !flag &&i<CANTBLOCKS; i++, sum++){
				if(sum==cant){
					return i-sum;
				}
				if(blocks[i]!=-1){
					flag=1;
				}
			}
		}
	}
	return 0;
}

void moveStack(int pid, int from, int to, int cant)
{
	int sc=process[current].ssize;
	from=from*BLOCKSIZE+BLOCKSIZE*cant;
	to=to*BLOCKSIZE+(BLOCKSIZE*(cant+1));
	void* source = (void*)((from+KERNEL_PAGES)*PAGE_SIZE);
	void* dest = (void*)((to+KERNEL_PAGES)*PAGE_SIZE);
	memcpy(source, dest, PAGE_SIZE*sc);
}

void moveHeap(int pid, int from, int to, int cant)
{
	int hc=process[current].hsize;
	from=1+(from*BLOCKSIZE);
	to=1+to*BLOCKSIZE;
	void* source = (void*)((from+KERNEL_PAGES)*PAGE_SIZE);
	void* dest = (void*)((to+KERNEL_PAGES)*PAGE_SIZE);
	memcpy(source, dest, PAGE_SIZE*hc);
}

void reallocMe(int pid, char which)
{
	printf("REALLOC\n\n");
	int i=getInitBlock(pid);
	int cant=getCantBlocks(i, pid);
	int where=getFreeSpace(cant+1);
	int stack=where*BLOCKSIZE+(BLOCKSIZE*(cant+1));
	int k;
	printf("habilito stack\n");
	for(k=stack; k>stack-process[current].ssize; k--)
	{
		page_table[k+KERNEL_PAGES]=(int*)((int)(page_table[k+KERNEL_PAGES])&0x00000001);		
		page_present[k]=1;	
	}
	if(which==ST){
		page_table[(k-1)+KERNEL_PAGES]=(int*)((int)(page_table[(k-1)+KERNEL_PAGES])&0x00000001);		
		page_present[k-1]=1;
	}
	printf("habilito heap\n");
	int heap = 1+where*BLOCKSIZE;
	for(k=heap; k<heap+process[current].hsize; k++){
		page_table[k+KERNEL_PAGES]=(int*)((int)(page_table[k+KERNEL_PAGES])&0x00000001);		
		page_present[k]=1;	
	}
	if(which==HP){
		page_table[(k+1)+KERNEL_PAGES]=(int*)((int)(page_table[(k+1)+KERNEL_PAGES])&0x00000001);		
		page_present[k+1]=1;
	}
	printf("move stack\n");
	moveStack(pid, i, where, cant);
	printf("move heap\n");
	moveHeap(pid, i, where, cant);
	int j=1+i*BLOCKSIZE;
	printf("libero\n");
	for(; j<cant*BLOCKSIZE; j++){
		page_present[j]=0;
		page_table[j+KERNEL_PAGES]=(int*)((int)(page_table[j+KERNEL_PAGES])&0xFFFFFFFE);
	}
}
