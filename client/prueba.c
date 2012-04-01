#include "strQueue.h"

int main()
{
	strQueue_t q = newQueue();
	queue(q, "hola");
	queue(q, "mundo");
	queue(q, "\n");
	printf("%s ",dequeue(q));
		printf("%s ",dequeue(q));

	printf("%s ",dequeue(q));

	if(dequeue(q)==NULL)
		printf("es null\n");
}