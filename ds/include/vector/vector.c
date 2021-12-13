#include <stdlib.h> /*  size_t dynamic memory allocation */
#include <stdio.h>   /*  standard library               */
#include <string.h>   /*  memmove                      */
#include <assert.h>    /* assert                      */ 
                                                 /* reviewed by Amit*/
#include "dynamic_vector.h"

#define GROWTHFACTOR (2)
#define ANDONE (1)


enum
{
	ERROR = -1,
	SUCCESS 
};

struct vector
{
	void *start;
	size_t capacity;
	size_t elem_size;
	size_t size;	
	
}; 


vector_t *VectorCreate(size_t element_size, size_t cap)
{
	vector_t *vector = NULL;
	
	
	vector = (vector_t *)malloc(sizeof(vector_t));
	vector->start = calloc(cap, element_size);

	assert (NULL != vector);
	assert (NULL != vector->start);

	vector->capacity = cap;
	vector->elem_size = element_size;
	vector->size = 0;

	return vector;
}


void VectorDestroy(vector_t *vec_ptr)
{
	free(vec_ptr->start);
	vec_ptr->start = NULL;
	free(vec_ptr);
	vec_ptr = NULL;
}


void *VectorGetAccessToElement(vector_t *vec_ptr, size_t index)
{
	return ((char*)vec_ptr->start + (vec_ptr->elem_size * (index - ANDONE))); 
}


int VectorPushBack(vector_t *vec_ptr, const void *element)
{
	void *start = NULL;

	if (NULL == vec_ptr)
	{
		return ERROR;
	}

	if ( ANDONE >= vec_ptr->capacity - vec_ptr->size)
	{
		vec_ptr = VectorReserve(vec_ptr, GROWTHFACTOR * vec_ptr->capacity);
	}

	start = memmove(((char*)vec_ptr->start + (vec_ptr->size * vec_ptr->elem_size)), element, vec_ptr->elem_size);
	
	assert (NULL != start);
	
	++(vec_ptr->size);
	return (vec_ptr == NULL);	
}


void VectorPopBack(vector_t *vec_ptr)
{

	if (vec_ptr->capacity - vec_ptr->size >= vec_ptr->size)
	{
		vec_ptr = VectorReserve(vec_ptr, vec_ptr->size + ANDONE);
	}
	--(vec_ptr->size);
}

size_t VectorSize(const vector_t *vec_ptr)
{
	return (vec_ptr->size);
	
}

size_t VectorCapacity(const vector_t *vec_ptr)
{
	return vec_ptr->capacity;
}


vector_t *VectorReserve(vector_t *vec_ptr, size_t new_size)
{
	void *start = NULL;
	
	if (NULL == vec_ptr)
	{
		return NULL;
	}

	if ((vec_ptr->size) > new_size)
	{
		++new_size;
	}
		
	start = realloc(vec_ptr->start, sizeof(vec_ptr->elem_size) * new_size);
	assert (NULL != start);

	vec_ptr->capacity = new_size; 
	vec_ptr->start = start;

	return vec_ptr;
}


