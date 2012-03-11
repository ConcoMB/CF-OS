#include "listasGen.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*Each Node, only can be seen by programmer*/
typedef struct node *nodeType; 

static nodeType insertNode( nodeType actualNode, listADT header,void *newElem);
static nodeType deleteNode( nodeType actualNode, listADT header,void* delElem);
static void listNode(nodeType actualNode, int pos,void (*printType)(void*));
static void freeNodes ( nodeType actualNode);
static void nodeIntersection(nodeType actualNode1, nodeType actualNode2,listADT headerResult);
static void nodeUnion(nodeType actualNode1, nodeType actualNode2,listADT headerResult);
		
struct listCDT /*Header*/
{
	int (*criteria)(void *,void*);
	int bytes;
	nodeType first;
	nodeType current;
};

struct node
{
	nodeType next;
	void *elem;
};


listADT
newList (int bytes, int (*function)(void*, void*))
{
	listADT newList;
	if( (newList=malloc(sizeof( struct listCDT ))) == 	NULL)
		return NULL;
	newList->bytes=bytes;
	newList->criteria=function;
	newList->first=NULL;
	newList->current=NULL;
	return newList;
}

void* getNext(listADT list)
{
    if(list->current!=null)
    {
        void* ans= list->current->elem;
        list->current=list->current->next;
        return ans;
    }
    return NULL;
}

void reset(listADT list)
{
    list->current=list->first;
}

void
insertElem( listADT header, void *newElem)
{
	header->first=insertNode(header->first,header,newElem);
}

static nodeType
insertNode( nodeType actualNode, listADT header, void *newElem)
{ 
	if ( actualNode == NULL || (*header->criteria)(newElem,actualNode->elem)  <0)
	{
		nodeType auxNode;
		if ( (auxNode=malloc(sizeof(struct node)) ) == NULL )
			return NULL;
		auxNode->elem=malloc(header->bytes);
		memcpy(auxNode->elem,newElem,header->bytes);/*agarra direccion de elem y auxNode->elem lo apunta a la direccion de elem*/
		auxNode->next=actualNode;
		return auxNode;
	}
	if( (*header->criteria)(newElem,actualNode->elem) !=  0 )
		actualNode->next=insertNode(actualNode->next,header,newElem);
	return actualNode;
}

void
deleteElem(listADT header, void*delElem)
{
	header->first=deleteNode(header->first,header,delElem);
}

static nodeType
deleteNode( nodeType actualNode, listADT header, void* delElem)
{
	if ( (*header->criteria)(delElem,actualNode->elem) == 0 )
	{
		nodeType auxNode;
		if ( (auxNode=malloc(sizeof(struct node))) == NULL)
			return NULL;
		auxNode=actualNode->next;
		free(actualNode);
		return auxNode;
	}
	if ( (*header->criteria)(delElem,actualNode->elem) >0)
		actualNode->next=deleteNode(actualNode->next,header,delElem);
	return actualNode;
}

void
getElem(listADT header, int pos,void (*printType)(void*))
{
	
	listNode(header->first, pos,printType);
}

static void
listNode(nodeType actualNode, int pos,void (*printType)(void*))
{
	if(pos == 0 && actualNode!=NULL )
	{
		(*printType)(actualNode->elem);
		return ;
	}
	if ( actualNode->next!=NULL)
		listNode(actualNode->next,--pos,printType);
	return ;
}

void
freeList( listADT header)
{
	freeNodes(header->first);
	free(header);
}

static void
freeNodes ( nodeType actualNode)
{
	if ( actualNode == NULL)
		return;
		freeNodes(actualNode->next);
		free(actualNode->elem);
		free(actualNode);
	return;
}

void *
listHead ( listADT header)
{
	void *aux;
	if ( header->first == NULL)
		return NULL;
	aux=malloc(header->bytes);
	memcpy(aux,header->first->elem,header->bytes);
	return aux;
}

listADT
intersection (listADT header1, listADT header2)
{
	listADT headerResult;
	headerResult=newList(header1->bytes,header1->criteria);
	nodeIntersection(header1->first,header2->first,headerResult);
	return headerResult;
}

static void
nodeIntersection(nodeType actualNode1, nodeType actualNode2,listADT headerResult)
{
	if ( actualNode1 != NULL && actualNode2 != NULL )
	{
		if ( (*headerResult->criteria)(actualNode1->elem,actualNode2->elem) == 0 )
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode1->elem);
			nodeIntersection(actualNode1->next,actualNode2->next,headerResult);
		}
		else if( (*headerResult->criteria)(actualNode1->elem,actualNode2->elem) <0 )
			nodeIntersection(actualNode1->next,actualNode2,headerResult);
		else
			nodeIntersection(actualNode1,actualNode2->next,headerResult);
	}
}

listADT
unionList ( listADT header1, listADT header2)
{
	listADT headerResult;
	headerResult=newList(header1->bytes,header1->criteria);
	nodeUnion(header1->first,header2->first,headerResult);
	return headerResult;
}

static void
nodeUnion(nodeType actualNode1, nodeType actualNode2,listADT headerResult)
{
	if ( actualNode1!= NULL && actualNode2!=NULL)
	{
		if ( (*headerResult->criteria)(actualNode1->elem, actualNode2->elem) <0 )
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode1->elem);
			nodeUnion(actualNode1->next,actualNode2,headerResult);
		}
		else if ( (*headerResult->criteria)(actualNode1->elem, actualNode2->elem) > 0)
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode2->elem);
			nodeUnion(actualNode1,actualNode2->next,headerResult);
		}
		else
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode1->elem);
			nodeUnion(actualNode1->next,actualNode2->next,headerResult);
		}
	}
	if ( actualNode1!=NULL)
	{
		while ( actualNode1!=NULL)
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode1->elem);
			actualNode1=actualNode1->next;
		}
	}
	if ( actualNode2!=NULL)
	{
		while ( actualNode2!=NULL)
		{
			headerResult->first=insertNode(headerResult->first,headerResult,actualNode2->elem);
			actualNode2=actualNode2->next;
		}
	}
}
