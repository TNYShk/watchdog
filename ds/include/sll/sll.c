#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/*
typedef struct slist slist_t;
typedef struct slist_node* slist_iter_t; 

the itterator uses slist_t and the node uses slist_iter_t ?
*/
#include "sll.h"


struct slist
{
    slist_iter_t head;
    slist_iter_t tail; /* tail is the dummy its moves */
    
};

struct slist_node
{
    void *data;
    slist_iter_t next;
};



slist_t *SListCreate(void)
{
	
	slist_t *slinky = NULL;
	slist_iter_t dummy = NULL;

	slinky = (slist_t *)malloc(sizeof(slist_t));
	dummy = (slist_iter_t)malloc(sizeof(struct slist_node));
	
	
	assert (NULL != slinky);
	assert (NULL != dummy);
	
	dummy->data = NULL;
	dummy->next = NULL;

	slinky->head = dummy;
	slinky->tail = dummy;
	
	return slinky;

}

void SListDestroy(slist_t *slist)
{
	slist_iter_t current =  slist->head;
	slist_iter_t temp =  NULL;

	while (current != slist->tail)
	{
		temp = current->next;
		free(current);
		current = temp;
	}
	free(slist->tail);
	free(slist);
}

/* create node, copy the data in to it, then change values of the data between the 2 nodes (where and new) then */

slist_iter_t SListInsertBefore(const slist_iter_t where, const void *data)
{
	slist_iter_t temp = NULL;
	temp = (slist_iter_t)malloc(sizeof(struct slist_node));
	temp->data = data;
	where->data = 


	return where;


}



slist_iter_t SListInsertAfter(slist_iter_t where, const void *data)
{
	slist_iter_t node_after = (slist_iter_t)malloc(sizeof(struct slist_node));
	
	node_after->data = memcpy(node_after->data,data,sizeof(node_after));
	node_after->next = where->next;
	
	where->next = node_after;

	return node_after;
}

void *SListGetData(const slist_iter_t iterator)
{
	assert (NULL != iterator);

	return (void*)iterator->data;
}

void SListSetData(slist_iter_t iterator, const void *data)
{
	assert(NULL != iterator );
	assert(NULL != data);
	iterator->data = data;
}

int SListIsEqual(const slist_iter_t iterator1, const slist_iter_t iterator2)
{
	
}