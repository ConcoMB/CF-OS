
#ifndef __Q__
#define __Q__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct strQueue_CDT* strQueue_t;


strQueue_t newQueue();

int queueStr(strQueue_t q, char* str);

char * dequeueStr(strQueue_t q);

int isEmpty(strQueue_t q);


#endif