
#ifndef __Q__
#define __Q__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct strQueue_CDT* strQueue_t;


strQueue_t newQueue();

int queueStr(strQueue_t q, char* fmt, ...);

char * dequeueStr(strQueue_t q);

int isEmpty(strQueue_t q);


#endif
