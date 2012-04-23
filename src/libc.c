#include "../include/libc.h"

int stack_count(){
	return __stack_count();
}


int pow(int b,int e){
	int i;
	int ans=1;
	for(i=0;i<e;i++){
		ans*=b;
	}
	return ans;
}



char gethour(){
	char h;
	__hour(&h);	
	//h-=3;
	return h;
}

char getmin(){
	char m;
	__min(&m);
	return m;
}



void memcpy(void* dest, void* source, int count){
	int i;
	for(i=0;i<count;i++){
		*(char*)(dest+i)=*(char*)(source+i);
	}
}

void* malloc(int bytes)
{
	return (void*)__malloc(bytes);
}

void* calloc(int bytes)
{
	return (void*)__calloc(bytes);
}

int free(void* page)
{
	return __free(page);
}

int heap_count(){
	return __heap_count();
}

void set_scancode(int i){
	__set_scancode(i);
}

int pageprint(int page){
	int i;
	int* adress = (int*)((530+page)*4096);
	if(page<0 || page>=1024-530){
		return 0;
	}
	for(i=0; i<4; i++){
		printf("%X\n",*adress);
		adress++;
	}
	return 1;
}

int kill(int pID)
{
	return __kill(pID);
}
