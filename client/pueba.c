#include "strQueue.h"
#include <pthread.h>
#include "printer.c"
#include <stdio.h>

strQueue_t queue;

int main()
{
	queue=newQueue();
	pthread_t print;
	pthread_create(&print, NULL, printThread, NULL);
	pthread_join(print, NULL);
	return 0;
}