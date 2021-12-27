/**********************************************
* Scheduler - Source Code                     *
* Developer: Tanya                   		  *
* 2021-12-                       		  	  *
*                                             *
*      Reviewed by    	 	  	 			  *
**********************************************/
#include <time.h>   /*time_t */
#include <stdlib.h> /* memory allocation  */
#include <assert.h> /* assert()         */
#include <string.h> /* memset         */


#include "task.h"
#include "scheduler.h" 
#include "priority_queue.h"


#define FAIL (-1)

struct scheduler
{
	pq_t *pq;
};

static int TaskMatchs(const void *task, const void *uid);




scheduler_t *SchedCreate(void)
{
    scheduler_t *new_sched = NULL;

    new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
    new_sched->pq = PQCreate(&TasksCompare);

    if (NULL == new_sched || NULL == new_sched->pq)
    {
        free(new_sched);
        new_sched = NULL;
    }

    return new_sched;
}

void SchedDestroy(scheduler_t *sched)
{
    PQDestroy(sched->pq);

    memset(sched, 0, sizeof(scheduler_t));
    
    free(sched);
    sched = NULL;
}

ilrd_uid_t SchedAddTask(scheduler_t *sched, task_func_t task_func, void *task_args,
                        cleanup_func_t cleanup_func, void *cleanup_args, time_t time_to_run)
{
    task_t *new_task = NULL;

    assert(NULL != sched);
    assert(NULL != task_func);
    assert((time_t)FAIL != time_to_run);

    new_task = TaskCreate(task_func, task_args, cleanup_func, cleanup_args, time_to_run);
    
    if(NULL == new_task)
    {
 
        return UIDBadUID;
    }

    if(0 < PQEnqueue(sched->pq, new_task))
        {
        
            TaskDestroy(new_task);
            new_task = NULL;
        }

    return TaskGetUID(new_task);

}

static int TaskMatchs(const void *task, const void *uid)
{
    return TaskIsMatch((const task_t *)task, *((ilrd_uid_t *)uid));
}

int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t uid)
{
    task_t *to_remove = NULL;

    assert(NULL != sched);
    assert(!UIDIsSame(UIDBadUID, uid));
   
   to_remove = PQErase(sched->pq, &TaskMatchs, &uid);
    
   if(NULL != to_remove)
   {
       
        TaskDestroy(to_remove);
   }
 
   return (NULL == to_remove);
   
}



size_t SchedSize(const scheduler_t *sched)
{
     assert(NULL != sched);

     return PQSize(sched->pq);
}


int SchedIsEmpty(const scheduler_t *sched)
{
    assert(NULL != sched);

    return PQIsEmpty(sched->pq);
}

void SchedClear(scheduler_t *sched)
{
    assert(NULL != sched);
    
    while (!SchedIsEmpty(sched))
    {
        TaskDestroy(PQDequeue(sched->pq));
    }
}