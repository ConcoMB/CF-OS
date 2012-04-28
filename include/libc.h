#ifndef __LIBC__
#define __LIBC__
#include "systemcalls.h"
#include "stdio.h"
#include "structs.h"

int stack_count();
int pow(int b,int e);
char gethour();
char getmin();
void memcpy(void* dest, void* source, int count);
void* malloc(int bytes);
void* calloc(int bytes);
int free(void* page);
int heap_count();
void set_scancode(int i);
int pageprint(int page);
void sleep(int ms);
int kill(int pID);

#endif
