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
}strQueue_CDT;




strQueue_t newQueue()
{
	strQueue_t q=malloc(sizeof(strQueue_CDT));
	if(q==NULL)
		return q;
	q->first=NULL;
	q->last=NULL;
	return q;
}

int queue(strQueue_t q, char* str)
{
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
	return 0;
}

char * dequeue(strQueue_t q)
{
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
