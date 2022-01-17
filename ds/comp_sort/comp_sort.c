/**********************************************
 * Comp_Sort - Source File                    *
 * Developer: Tanya			                  *
 *          Jan 12, 2022                      *
 *                                            *
 * Reviewer:Amit Shlomo	                      *
 **********************************************/
#include <stddef.h> /* size_t*/
#include <assert.h> /* assert*/
#include <stdio.h> /* print test */
#include "comp_sort.h"

/* Service func */
static void PSwap(int *i , int *j);
static int FindMinIndex(int *arr, size_t len);


void BubbleSort(int *arr, size_t arr_size)
{
	
	size_t i = 0;

	assert (NULL != arr);

	while(arr_size)
	{
		for(i = 1; i < arr_size ; ++i)
		{
			if (arr[i - 1] > arr[i])
			{
				PSwap(&arr[i - 1], &arr[i]);
			}
		}
	--arr_size;
	}
	
}



void SelectionSort(int *arr, size_t arr_size)
{
	int i = 0;
	int *runner = arr;
	int *end = arr + arr_size;

	assert(NULL != arr);
	

   for(i = 0; runner < end; ++runner, ++i)
   {
    	int min = FindMinIndex(runner, arr_size - i);
    	
    	PSwap(runner, runner + min);  
	}
}


void InsertionSort(int *arr, size_t arr_size)
{
	size_t index = 1;

	for (; index < arr_size ; ++index)
	{
		size_t inner_index = index;

		while( inner_index > 0 && arr[inner_index - 1] > arr[inner_index])
		{
			PSwap(&arr[inner_index], &arr[inner_index - 1]);
			--inner_index;
		}
	}
}



static void PSwap(int *i , int *j)
{
	int holder = *j;

	assert (NULL != j);
	assert (NULL != i);

	*j = *i;
	*i =  holder;
}

static int FindMinIndex(int *arr, size_t len)
{
	int *start = arr;
	int *end = arr + len;
	int *lowest = start;
	
	
	while(start < end)
	{
		if(*lowest > *start)
		{
			lowest = start;
			
		}
		++start;
	}
	return lowest - arr;
}



