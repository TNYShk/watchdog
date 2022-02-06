#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */

typedef struct heap heap_t;
typedef int (*heap_cmp_func_t)(const void *data1, const void *data2);
typedef int (*heap_is_match_func_t)(const void *element, const void *param);

/*for c file
#include "vector.h"

struct heap
{
    heap_cmp_func_t cmp_func;
    vector_t *vec;
};
*/

/* complexity: O(1) Space: O(1)*/
heap_t *HeapCreate(heap_cmp_func_t cmp_fun); 

/* complexity: O(1) Space: O(1)*/
void HeapDestroy(heap_t *heap);

/* complexity: O(log n) Space: O(1)*/
int HeapPush(heap_t *heap, void *data);

/* complexity: O(log n) Space: O(1)*/
void HeapPop(heap_t *heap);

/* complexity: O(1) Space: O(1)*/
void *HeapPeek(const heap_t *heap);

/* complexity: O(1) Space: O(1)*/
size_t HeapSize(const heap_t *heap);

/* complexity: O(1) Space: O(1)*/
int HeapIsEmpty(const heap_t *heap);

/* complexity: O(log n) Space: O(1)*/
void *Remove(heap_t *heap, heap_is_match_func_t match_func, void *param);

#endif /* __HEAP_H__ */
