#ifndef __SYSCALLS__
#define __SYSCALLS__
#include "structs.h"
void __hour(char* hp);
void __min(char* mp);
void* __malloc(int bytes);
void* __calloc(int bytes);
int __free(void* i);
int __stack_count();
void __setcolor(char* colorp);
void __set_scancode(int i);
int __heap_count();
void __sleep(int ms);
int __kill(int pID);
void* __top();
void __createChild(int (*func)(int, char**), int argc, char ** argv);

/* __write
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
*
**/
int __write(int fd, const void* buffer, int count);

/* __read
*
* Recibe como parametros:
* - File Descriptor
* - Buffer a donde escribir
* - Cantidad
*
**/
int __read(int fd, void* buffer, int count);
int __readNB(int fd, void* buffer, int count);


#endif
