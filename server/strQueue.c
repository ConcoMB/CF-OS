#include "strQueue.h"

typedef struct elem_t
{
	struct elem_t* next;
	char * string;
}elem_t;


typedef struct strQueue_CDT
{
	struct elem_t* first;
	struct elem_t* last;
	sem_t* sem;
}strQueue_CDT;




strQueue_t newQueue()
{
	strQueue_t q=malloc(sizeof(strQueue_CDT));
	if(q==NULL)
		return q;
	q->first=NULL;
	q->last=NULL;
	q->sem=sem_open("/semPrint", O_RDWR|O_CREAT, 0666, 0);
	return q;
}

int queueStr(strQueue_t q, char* fmt, ...)
{
	char str[150];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(str,fmt,ap);
	va_end(ap);
	elem_t * newElem = malloc(sizeof(elem_t));
	newElem->string=malloc(strlen(str)+1);
	if(newElem->string==NULL)
		return 1;
	strcpy(newElem->string, str);
	newElem->next=NULL;
	if(isEmpty(q))
	{
		q->first=newElem;
	}
	else
	{
		q->last->next=newElem;
	}
	q->last=newElem;
	sem_post(q->sem);
	return 0;
}

char * dequeueStr(strQueue_t q)
{
	sem_wait(q->sem);
	if(isEmpty(q))
	{
		return NULL;
	}
	elem_t* oldElem = q->first;
	q->first=oldElem->next;
	char * ans = oldElem->string;
	oldElem->next=NULL;
	free(oldElem);
	return ans;
}

int isEmpty(strQueue_t q)
{
	return q->first==NULL;
}
