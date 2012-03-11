/*
 * genlistADT.c
 *
 */

/* Version Iterativa de listas con header */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"



struct listCDT
{
	int (*fComp) (void *, void *);	/* funcion de comparacion */
	struct listNode * first;	/* puntero al primer nodo de la lista */
	struct listNode * current;	/* puntero al actual, para iterar */
	int size;			/* cantidad de nodos */
};

typedef struct listNode
{
	listElementT data;
	struct listNode * next;
}listNode;


static void
error(const char* s)
{
	fprintf(stderr, s);
	exit(EXIT_FAILURE);
}


listADT
newList( int (*fComp) (void *, void *) )
{
	listADT l = malloc(sizeof( struct listCDT));
	if ( l == NULL)
		return NULL;
	l->fComp = fComp;
	l->first = NULL;
	l->current = NULL;
	l->size = 0;
}


int
listIsEmpty( listADT list)
{
	return list->size == 0;
	/* 
	return list->first == NULL
	*/
}


void
reset( listADT list )
{
	list->current = list->first;
}

listElementT
getNext( listADT list)
{
	listNode * current;
	
	if ( list->current == NULL )
		return NULL;
	
	current= list->current;
	list->current = current->next;
	
	return current->data;
}




int
elementBelongs( listADT list, listElementT element)
{
	listNode * node;
	int cmp;
	node = list->first;

	/* Como no sabemos la complejidad de la funcion de comparacion, conservamos 
	** el resultado en una variable.
	*/
	while( node != NULL &&   (cmp = (*list->fComp) (node->data, element)) == -1)
		node = node->next;

	return node != NULL && cmp == 0;
}


/* Funcion auxiliar para insertar en la lista de nodos, en forma analoga
** a insertar en una lista lineal sin header
*/
static int
insertNode( listNode ** list, listElementT element, int (*f)(void *, void*))
{

	/* Inserto al final o delante del actual porque es mayor o igual*/
	if( *list == NULL || (*f)((*list)->data, element) == 1 || (*f)((*list)->data, element) == 0)
	{
		listNode * auxi = malloc(sizeof( listNode ));
		if (auxi == NULL)
			Error("No hay lugar para otro nodo\n");
		auxi->next = *list;
		auxi->data = element;
		*list = auxi;
		return 1;
	}


	/* El elemento actual es menor */
	return insertNode( &((*list)->next), element, f);


}


void
insert( listADT list, listElementT element)
{
	if (element == NULL)
		return;

	list->size += insertNode(&(list->first), element, list->fComp);

}


static int
deleteNode( listNode **list, listElementT element, int (*f)(void *, void*))
{
	if( *list == NULL || (*f)((*list)->data, element) == 1 )
		return 0;

	if( (*f)((*list)->data, element) == 0 )
	{
		listNode * aux = *list;
		*list = (*list)->next;
		free(aux);
		return 1;
	}

	/* El elemento actual es menor */
	return deleteNode( &((*list)->next), element, f);

}

int
delete( listADT list, listElementT element)
{
	int res;
	res = deleteNode(&(list->first), element, list->fComp);
	list->size -= res;
	return res;
}


int
size( listADT list )
{
	return list->size;
}

void
freeList( listADT list)
{
	listNode * auxi, * node = list->first;

	while ( node != NULL )
	{
		auxi = node->next;
		free(node);
		node = auxi;
	}

	/* Liberamos el header */
	free(list);
}
