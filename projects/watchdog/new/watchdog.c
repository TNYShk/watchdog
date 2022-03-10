/********************************************
 * WATCHDOG - Source Code                   *
 * Developer: Tanya                   		*
 * Mar 8, 2022                   	     	*
 *                                          *
 *      Reviewed by        	 	  	 		*
*********************************************/
#define _XOPEN_SOURCE (700)
#define _POSIX_C_SOURCE 200112L 
#include <time.h>        /* time_t      */
#include <assert.h>      /* assert      */
#include <pthread.h>     /* thread_t    */
#include <signal.h>      /* sigaction   */
#include <sys/types.h>   /* pid_t       */
#include <unistd.h>      /* fork        */
#include <stdio.h>       /* printf      */
#include <stdlib.h>      /* atoi        */
#include <errno.h>       /* errno       */
#include <string.h>      /* strlen      */
#include "scheduler.h"		  /* scheduler API        */
#include "semaphore_sys_v.h"  /* sys_v sempahore API  */
#include "watchdog.h"         /* watchdog API         */

#ifdef DEBUG
	#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#else
	#define errExit(msg) do { perror(msg); return(errno); } while (0)
#endif

#define atomic_sync_or_and_fetch(destptr, flag) __sync_or_and_fetch(destptr, flag)
#define atomic_compare_and_swap(destptr, oldval, newval) __sync_bool_compare_and_swap(destptr, oldval, newval)


	
#define PING_EVERY (1)
#define CHECK_ALIVE_EVERY (5)
#define SUCCESS (0)
#define FAIL (-1)
#define ARGZ (256)
#define PATHNAME ("./kickdog")

static void *WrapperSchedSem(void *something);
static void SomeFailDie(scheduler_t *sched);
static void SigHandlerAlive(int sig, siginfo_t *info, void *ucontext);
static void SigHandlerKill(int sig);
static int TaskPingAlive(void *args);
static int TaskCheckAlive(void *args);
static int TaskStopSched(void *pid);

static int InitSched(void);
static int InitProcess(char *argv[]);
static int InitSigHandler(void);
static int InitSemaphore(char *argv[]);

static int Revive(char *argv[]);

typedef struct revive
{
	char buffer[ARGZ];
	char *whole;
	pid_t pid_child;
}revive_t;

static scheduler_t *new_sched;
static int alive_g = 0;
static int semid;
static int sched_flag = 0;
static pthread_t watchdog_t_g = {0};

revive_t revive_g;
char semchar[ARGZ] = {0};

int im_watchdog = 0;

int WDStart(int argc, char *argv[])
{
	revive_g.pid_child = getpid();
	InitSigHandler();

	(void)argc;
	
	assert(SUCCESS == InitSemaphore(argv));
	assert(SUCCESS == InitSched());

	
	

	
    if(0 == im_watchdog)
	{
		InitProcess(argv);
		SemDecrement(semid,1);
	}
	if(im_watchdog)
	{
		SchedRun(new_sched);
	}
	else if(SUCCESS != pthread_create(&watchdog_t_g, NULL, &WrapperSchedSem, new_sched))
	{
		SomeFailDie(new_sched);
		write(STDOUT_FILENO, "pthread create watchdog SIGUSR2\n", 25);
		kill(revive_g.pid_child, SIGUSR2);
		errExit("pthread_create");
	}

	return SUCCESS;
}

static int InitSigHandler(void)
{
	struct sigaction sa = {0};
	struct sigaction ka = {0};
	
	sa.sa_sigaction = &SigHandlerAlive;
	ka.sa_handler = &SigHandlerKill;
    sa.sa_flags |= SA_SIGINFO;
	
    if (SUCCESS != sigaction(SIGUSR1, &sa, NULL))
    {
        errExit("Failed to set SIGUSR1 handler");
    }

    if (SUCCESS != sigaction(SIGUSR2, &ka, NULL))
    {
        errExit("Failed to set SIGUSR2 handler");
    }
	return SUCCESS;
}

static int InitSemaphore(char *argv[])
{
	char *name_sem = NULL;
	
	if(NULL != (name_sem = getenv("SEMV")))
	{
		semid = atoi(name_sem);
		return SemIncrement(semid,1);
	}
	else
	{
		semid = InitSem(0);
			if(0 > semid)
			{
				errExit("Init_sem");
			}
		sprintf(semchar,"%d", semid);
		setenv("SEMV",semchar,1);
	}
	return SUCCESS;
}

void WDStop(void)
{
	if(0 == im_watchdog )
	{
		write(STDOUT_FILENO, "WDStop\n", strlen("WDStop "));
		kill(revive_g.pid_child, SIGUSR2);
		/* kill(getppid(), SIGUSR2); */
		pthread_join(watchdog_t_g, NULL);

			SemRemove(semid); 
			unsetenv("SEMV");
	}
	
	SchedDestroy(new_sched);
    new_sched = NULL;
   
}



static int InitProcess(char *argv[])
{
	
	revive_g.pid_child = fork();
		
	if(0 > revive_g.pid_child)
	{
		errExit("fork fail");
	}
	if (0 == revive_g.pid_child) /* in watchdog process */
	{
		if( execvp((im_watchdog == 0)? PATHNAME : *argv ,argv))
		{
			errExit("Failed execvp");
		}
	}

    memcpy(revive_g.buffer, argv[0], strlen(argv[0]));
    revive_g.whole = revive_g.buffer + strlen(revive_g.buffer) + 1;
 	strcat(revive_g.whole, semchar);

	return SUCCESS;
}

static int InitSched(void)
{
	new_sched = SchedCreate();
	if(NULL == new_sched)
	{
		errExit("SchedCreate failed");
	}

	if(UIDIsSame(UIDBadUID,SchedAddTask(new_sched, &TaskPingAlive, NULL, 
    						NULL, NULL, time(0) + PING_EVERY)))
    {
    	SomeFailDie(new_sched);
    	errExit("UIDBadUID == SchedAddTask");
    	
    }
    if(UIDIsSame(UIDBadUID,SchedAddTask(new_sched, &TaskCheckAlive, NULL, 
    					NULL, NULL, time(0) + CHECK_ALIVE_EVERY)))
    {
    	SomeFailDie(new_sched);
    	errExit("UIDBadUID == SchedAddTask");
    }

    if(UIDIsSame(UIDBadUID,SchedAddTask(new_sched, &TaskStopSched, NULL, 
    					NULL, NULL, time(0) + CHECK_ALIVE_EVERY)))
    {
    	SomeFailDie(new_sched);
    	errExit("UIDBadUID == SchedAddTask");
    }

	printf("in WATCHDOG ppid is %d, and pid is %d\n", getppid(), getpid()); 
	return SUCCESS;
}




static void SomeFailDie(scheduler_t *sched)
{
    SchedDestroy(sched);
    sched = NULL;
   	SemRemove(semid); 
}

static void *WrapperSchedSem(void *something)
{
	printf("schedrun? %d\n",SchedRun((scheduler_t *)something));
	return NULL;
}


static int TaskPingAlive(void *args)
{
	(void)args;

	if(0 != revive_g.pid_child)
	{
		kill(revive_g.pid_child, SIGUSR1);
	}

	return PING_EVERY;
}

static int TaskCheckAlive(void *args)
{
	(void)args;
	if(!__sync_bool_compare_and_swap(&alive_g, 1, 0))
	{
		write(STDOUT_FILENO, "REVIVE PLEASE\n", strlen("REVIVE PLEASE "));
		Revive(args);
	}
	
	return CHECK_ALIVE_EVERY;
}

static int Revive(char *argv[])
{
	
	return InitProcess(argv);
}


static void SigHandlerAlive(int sig, siginfo_t *info, void *ucontext)
{
	(void)sig;
	(void)ucontext;	
	write(STDOUT_FILENO, "Hungry Barking DOG\n", strlen("Hungry Barking DOG  "));
	atomic_sync_or_and_fetch(&alive_g, 1);
	revive_g.pid_child = info->si_pid;
}

static void SigHandlerKill(int sig)
{
	(void)sig;
	
	atomic_compare_and_swap(&sched_flag, 0 , 1);
}

int TaskStopSched(void *pid)
{
	(void)pid;
	if (1 == sched_flag)
	{
		SchedStop(new_sched);
	}
	return PING_EVERY;
}

