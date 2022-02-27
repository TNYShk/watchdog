/***********************************
 * Semaphore_posix - Source File   *
 * Developer: Tanya                *
 *          Feb 25, 2022           *
 *                                 *
 * Reviewer:                       *
************************************/
#include <fcntl.h>  /* O_* const.*/
#include <sys/stat.h> /* mode const. */
#include <semaphore.h>
#include <stdio.h>
#include <pthread.h> /* threads.. */
#include <assert.h> /* assert*/
#include <unistd.h> /* sleep */
#include <stdlib.h> /* atoi */
#include <errno.h>
#include <signal.h>
#include <string.h> /*strcmp */
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include "semaphore_posix.h"



#define FAIL (-1)
#define GREAT_SUCCESS (0)



typedef int (*sem_act_func)(sem_t *sem, const char **cmd);
static sem_act_func sem_actions[5] = {NULL};



static void InitSemAct(void);
static int DoExit(sem_t *sem, const char **cmd);
static int DoView(sem_t *sem, const char **cmd);
static int DoUnlink(sem_t *sem, const char **cmd);
static int DoDecrement(sem_t *sem, const char **cmd);
static int DoIncrement(sem_t *sem, const char **cmd);
static sem_t *InitSemaphore(const char **cmd);


static void InitSemAct(void)
{
    sem_actions['X'] = &DoExit;
    sem_actions['V'] = &DoView;
    sem_actions['I'] = &DoIncrement;
    sem_actions['D']= &DoDecrement;
    sem_actions['R'] = &DoUnlink;
}

static sem_t *InitSemaphore(const char **cmd)
{
    return sem_open(cmd[1], O_CREAT, S_IRUSR | S_IWUSR, atoi(cmd[2]));
}

int PosixSemManipulation(const char **cmd)
{
    sem_t *semP = NULL;

    semP = InitSemaphore(cmd);
    if(SEM_FAILED == semP)
    {
        errExit("sem_open");
    }
    
    InitSemAct();
    system(" ls -al /dev/shm/sem.*|more ");

    return sem_actions[(size_t)cmd[2][0]] (semP,cmd);
}

static int DoExit(sem_t *sem, const char **cmd)
{
    if(FAIL == sem_close(sem))
        errExit("sem_close");
    
    (void)cmd;
    return GREAT_SUCCESS;
}

static int DoView(sem_t *sem, const char **cmd)
{
    int val = 0;
    sem = sem_open(cmd[1], O_EXCL);
    if( FAIL == sem_getvalue(sem, &val))
        errExit("sem_getvalue");
    
    printf("getval view is %d\n", val);
    return GREAT_SUCCESS;
}

static int DoUnlink(sem_t *sem, const char **cmd)
{
    sem = sem_open(cmd[1], O_EXCL);
    if( FAIL == sem_unlink(cmd[1]))
        errExit("sem_unlink");
    
    (void)sem;
    return GREAT_SUCCESS;
}

static int DoDecrement(sem_t *sem, const char **cmd)
{
    int sem_val = atoi(cmd[2]);
    sem = sem_open(cmd[1], O_EXCL);
    if( EAGAIN == sem_trywait(sem))
        errExit("sem_trywait");
    sem_val = atoi(cmd[2]);
    printf("post dec, val is is %d\n", sem_val);
    printf("%ld sem_wait() succeeded\n", (long) getpid());
    return GREAT_SUCCESS;
}

static int DoIncrement(sem_t *sem, const char **cmd)
{
    sem = sem_open(cmd[1], O_EXCL);
    if( FAIL == sem_post(sem))
        errExit("sem_post");
    
    (void)cmd;

    return GREAT_SUCCESS;
}
