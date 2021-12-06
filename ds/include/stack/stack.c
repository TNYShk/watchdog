#include <stdlib.h> /* size_t, dyncamic memory allocation  */
#include <stdio.h>
#include <assert.h> /* asserts */
#include <limits.h> /* CHAR_BIT */
#include <string.h> /* memcpy*/

#include "stack.h"

#define TOTALCAP (CHAR_BIT * sizeof(size_t))

struct stack
{
	void *top;
	size_t capacity; 
	size_t elem_size;
	void *base;

}; 



stack_t *StackCreate(size_t num_of_elements, size_t element_size)
{
	stack_t *sptr = NULL;
	sptr = (stack_t *)malloc(sizeof(stack_t));
	
	sptr->base = malloc(num_of_elements * element_size);

	if ((NULL != sptr) && (NULL != sptr->base))
		{ 

		sptr->capacity = num_of_elements;
		sptr->elem_size = element_size;
	
		sptr->top = sptr->base;
	}

	return sptr;


}

void StackPush(stack_t *sptr, const void *element)
{
	memcpy(sptr->top, element, (sptr->elem_size));

	sptr->top = (void *)((size_t *)sptr->top + sptr->elem_size);
}


int StackIsEmpty(const stack_t *sptr)
{
	return (sptr->top == sptr->base);
}

void StackDestroy(stack_t *sptr)
{
	free(sptr->base);
	free(sptr);
	sptr->base = NULL;
	sptr = NULL;
}

size_t StackSize(const stack_t *sptr)
{
	return(((size_t)sptr->top - (size_t)sptr->base)/TOTALCAP);
}

void *StackPeek(const stack_t *sptr)
{
	return (void *)((size_t *)sptr->top - sptr->elem_size);
}

void StackPop(stack_t *sptr)
{
	
	sptr->top = (void *)((size_t *)sptr->top - (size_t)sptr->elem_size);
}

size_t StackCapacity(const stack_t *sptr)
{
	return (sptr->capacity);
}









	


