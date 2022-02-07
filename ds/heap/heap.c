/**********************************
 * Heap - Source Code          	  *
 * Developer: Tanya               *
 * Feb 10, 2022                   *
 *                                *
 *      reviewed by               *
***********************************/
#include <stdlib.h> /*  size_t dynamic memory allocation */
#include <stdio.h>   /*  standard library               */
#include <string.h>   /*  memset                      */
#include <assert.h>    /* assert                      */ 

#include "heap.h"
#include "dynamic_vector.h"


#define VCAP (16)
#define ELEM_S (sizeof(size_t))
#define HEAPROOT (0)



struct heap
{
    heap_cmp_func_t cmp_func;
    vector_t *vec;
};


static void HeapifyDown(heap_t *heap,size_t idx);
static void HeapifyUp(heap_t *heap, size_t idx);
static void **GetRightChild(vector_t *vec, size_t idx);
static void **GetLeftChild(vector_t *vec, size_t idx);

static void GenericSwap(char *left, char *right, size_t size);
static void PrintHeap(heap_t *heap);
static void PSwap(void **left, void **right);




heap_t *HeapCreate(heap_cmp_func_t cmp_fun)
{
    heap_t *heap = NULL;

    assert(NULL != cmp_fun);

    heap = (heap_t *)malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return NULL;
    }

    heap->vec = VectorCreate(ELEM_S, VCAP);
    if (NULL == heap->vec)
    {
        free(heap);
        return NULL;
    }

    heap->cmp_func = cmp_fun;

    return heap;

}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    VectorDestroy(heap->vec);

    memset(heap,0, sizeof(heap_t));
    free(heap);
    heap = NULL;
}


int HeapPush(heap_t *heap, void *data)
{
    size_t cur = 0;

    assert(NULL != heap);
    assert (NULL != data);

    cur = HeapSize(heap);

    VectorPushBack(heap->vec, &data);
    HeapifyUp(heap, HeapSize(heap) - 1);
    
    PrintHeap(heap);

    return (cur == HeapSize(heap));
}

void HeapPop(heap_t *heap)
{
    void **last = NULL;
    void **root = NULL;

    assert(NULL != heap);
    
    last = VectorGetAccessToElement(heap->vec, HeapSize(heap) -1);
    root = VectorGetAccessToElement(heap->vec, HEAPROOT);
   
    PSwap(root, last);
    VectorPopBack(heap->vec);
    HeapifyDown(heap, HEAPROOT);
       
}


int HeapIsEmpty(const heap_t *heap)
{
    assert (NULL != heap);

    return !VectorSize(heap->vec);
}

size_t HeapSize(const heap_t *heap)
{
    return (heap == NULL)? 0: VectorSize(heap->vec);
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);

    return *(void **)VectorGetAccessToElement(heap->vec,HEAPROOT);
}


void *HeapRemove(heap_t *heap, heap_is_match_func_t match_func, void *param)
{
    size_t idx = 0;
    void **last = VectorGetAccessToElement(heap->vec, HeapSize(heap) -1);
    void *removed_data = NULL;

    while(!match_func(*(void **)VectorGetAccessToElement(heap->vec, idx), param) && (idx < HeapSize(heap)))
    {
       ++idx;
    }
    if (match_func(*(void **)VectorGetAccessToElement(heap->vec, idx), param))
    {
        PSwap(VectorGetAccessToElement(heap->vec, idx), last);

        removed_data = *(void **)VectorGetAccessToElement(heap->vec, (HeapSize(heap) - 1));
        VectorPopBack(heap->vec);

        HeapifyDown(heap,idx);
    }
    
    PrintHeap(heap);
    return ((removed_data == NULL)? NULL: removed_data);
}



static void PSwap(void **left, void **right)
{
    void *holder = *left;
    *left = *right;
    *right = holder;
}


static void GenericSwap(char *left, char *right, size_t size)
{
    while(0 < size)
    {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        ++left;
        ++right;
        --size;
    }
}

static void HeapifyUp(heap_t *heap, size_t new_idx)
{
    size_t parent_idx = ((new_idx - 1) >> 1);

    void **parent_data = NULL;
    void **data = NULL;
    
    if(0 == new_idx)
        return;

    data = VectorGetAccessToElement(heap->vec, new_idx);
    parent_data = VectorGetAccessToElement(heap->vec, parent_idx);

    if ( 0 > heap->cmp_func(*data, *parent_data) )
    {
        PSwap(VectorGetAccessToElement(heap->vec, new_idx), parent_data);
        HeapifyUp(heap, parent_idx);
    }
}


/*
static void HeapifyDown(heap_t *heap, size_t idx)
{
    void **left_child = NULL, **right_child = NULL;
    int where = 0;
    void **data = NULL;

    left_child = GetLeftChild(heap->vec, idx);
    right_child = GetRightChild(heap->vec,idx);
    data = VectorGetAccessToElement(heap->vec, idx);

    if (left_child == NULL)
    {                     
        return;
    }
     if (right_child == NULL)
    {                     
        PSwap(data,left_child);
        return;
    }

    where = heap->cmp_func(*left_child, *right_child);
    if(0 == where)
        return;

    if(0 < where)
    {
        PSwap(data,right_child); 
        HeapifyDown(heap,((idx * 2) + 2)); 
    }
    else
    {
        PSwap(data,left_child);
        HeapifyDown(heap,((idx * 2) + 1));   
    }
}

*/

static void HeapifyDown(heap_t *heap, size_t idx)
{
    size_t len = HeapSize(heap);
    size_t left = (idx << 1) + 1;
    size_t right = (idx << 1) + 2;
    
    size_t cur_idx = idx;

    void **min_data = NULL;
    void **data = NULL;

    min_data = VectorGetAccessToElement(heap->vec, cur_idx);
    if (len > left && (data = VectorGetAccessToElement(heap->vec, left), heap->cmp_func(*data, *min_data) < 0))
    {
        cur_idx = left;
        min_data = data;
    }
    if (len > right &&
        (data = VectorGetAccessToElement(heap->vec, right),
            heap->cmp_func(*data, *min_data) < 0))
    {
        cur_idx = right;
        min_data = data;
    }

    if (idx != cur_idx)
    {
        data = VectorGetAccessToElement(heap->vec, idx);
        PSwap(data, min_data);
        HeapifyDown(heap, cur_idx);
    }
}


static void **GetLeftChild(vector_t *vec, size_t idx)
{
    if(idx < VectorSize(vec))
    {
        return VectorGetAccessToElement(vec, ((idx * 2) + 1));
    }
    return NULL;
    
}

static void **GetRightChild(vector_t *vec, size_t idx)
{
    if(idx < VectorSize(vec))
    {
        return VectorGetAccessToElement(vec, ((idx * 2) + 2));
    }
    return NULL;
}

static void PrintHeap(heap_t *heap)
{
    size_t i = 0;
    printf("\n");
    for (; i < HeapSize(heap); ++i)
    {
        printf("%ld, ", **(int **)VectorGetAccessToElement(heap->vec, i));
    }
}



