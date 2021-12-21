/***********************************
 * Sorted Linked List WS		   *
 *           	   *
 * by Tanya					       *
 *   			           *
 *                                 *
 * Reviewer:                 *
 ***********************************/

#include <stdio.h> /* printf, size_t  */
#include <stdlib.h> /* memory allocation  */
#include <assert.h> /* assert() */
#include <string.h>
#include "dll.h"
#include "sorted_list.h"



#define PRINT_NODE(node) (printf("node at address %p has int %d, prev is %p, next is %p\n", \
        (void *)node, *(int *)DListGetData(node), (void *)DListPrev(node), (void *)DListNext(node)));


static void PrintSListForward(const sort_list_t *slist)
{
    sort_list_iter_t runner;
    
    assert(NULL != slist);
    
    runner = SortListBegin(slist);
    
    printf("\n-------List is:------\n");
    
    while (!SortListIterIsEqual(runner, SortListEnd(slist)))
    {
        /*printf("in while"); */
        printf("%d ", *(int *)SortListGetData(runner));
        runner = SortListNext(runner);
    }
    printf("\n\nSize of list is: %lu. List %s.\n", SortListSize(slist), SortListIsEmpty(slist) ? "is empty" : "is not empty");
    printf("-------list ends------\n");
}

/*
typedef int (*cmp_func_t)(const void *left, const void *right);

typedef struct sort_list sort_list_t;
typedef struct sort_list_iter sort_list_iter_t;

struct sort_list_iter
{
    dlist_iter_t diter;            
#ifdef DEBUG
    sort_list_t *slist;
#endif
};
*/

int CompareData(const void *left, const void *right);

void TestOne();
void TestTwo();

int main (void)
{
    
    TestOne();
    TestTwo();
    return 0;
}

void TestOne()
{
    size_t x = 6;
    size_t y = 8;
    size_t one = 1;
    size_t s = 7;
    size_t ans = 0;
    void *ptr = NULL;
    sort_list_t *test = NULL;
    sort_list_iter_t iter1;
    test = SortListCreate(CompareData);
  
    printf("\n\t***********************-Test-1-*********************************\n\n");
    printf("\t****create sorted list, insert and pop front and back***********\n");
     
    iter1 = SortListInsert(test, &one);
    iter1 = SortListInsert(test, &y);
    iter1 = SortListInsert(test, &y);
    iter1 = SortListInsert(test, &s);
    iter1 = SortListInsert(test, &one);
    iter1 = SortListInsert(test, &x);
    
    
    PrintSListForward(test);
   
   (SortListSize(test) > 0)? printf("size is %ld\n",SortListSize(test)) : printf("empty list\n");
  
   (SortListIsEmpty(test) == 1) ? printf("IsEmpty -empty list\n") : printf("Not empty list\n");
   ptr = SortListPopFront(test);
   printf("pop front data is %ld\n",*(size_t*)ptr);
   ptr = SortListPopBack(test);
   printf("pop back data is %ld\n",*(size_t*)ptr);
   
    (SortListSize(test) > 0)? printf("size is %ld\n",SortListSize(test)) : printf("empty list\n");
    PrintSListForward(test);
    printf("\t**************insert and remove node********************************\n");
    ans = y * s;
    iter1 = SortListInsert(test, &ans);
    PrintSListForward(test);
    SortListRemove(iter1);
    PrintSListForward(test);
    
    SortListDestroy(test);
}

void TestTwo()
{
    size_t x = 60;
    size_t y = -88;
    size_t one = 11;
    size_t s = 77;
    size_t ans = 0;
    void *ptr = NULL;
    sort_list_t *test = NULL;
    sort_list_t *test2 = NULL;
    sort_list_iter_t iter1, iter2;
    test = SortListCreate(CompareData);
    test2 = SortListCreate(CompareData);
  
    printf("\n\t***********************-Test-2-*********************************\n\n");
    printf("\t****create 2 sorted lists, insert in them ***********\n");
     
    iter1 = SortListInsert(test, &one);
    iter1 = SortListInsert(test, &y);
    iter1 = SortListInsert(test, &y);
    iter1 = SortListInsert(test, &s);
    iter1 = SortListInsert(test, &ans);
    iter1 = SortListInsert(test, &x);
    
    (SortListSize(test) > 0)? printf("size is %ld\n",SortListSize(test)) : printf("empty list\n");

    iter2 = SortListInsert(test2, &s);
    iter2 = SortListInsert(test2, &ans);
    iter2 = SortListInsert(test2, &x);
    
    (SortListSize(test2) > 0)? printf("size is %ld\n",SortListSize(test2)) : printf("empty list\n");
    
    printf("\t**************Before Merge********************************\n");
    printf("\t**************list1 src********************************\n");
    PrintSListForward(test);
    printf("\t**************list2 dest********************************\n");
    PrintSListForward(test2);
     
    printf("\t**************Post Merge********************************\n");
    SortListMerge(test2, test);
    
   
    printf("\t**************list1 src********************************\n");
    PrintSListForward(test);
    printf("\t**************list2 dest********************************\n");
    PrintSListForward(test2);
    printf("\t****insert to list1 src ***********\n");
    iter1 = SortListInsert(test, &ans);
    PrintSListForward(test);
    printf("\t****insert to list2 src ***********\n");
    iter2 = SortListInsert(test2, &ans);
    PrintSListForward(test2);
    SortListDestroy(test);
    SortListDestroy(test2);


}