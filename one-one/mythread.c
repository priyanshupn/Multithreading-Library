#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/syscall.h>
#include "mythread.h"
static struct thread **threads[20] = {0};
static int _threadCount = 0;
static inline pid_t(gettid)
{
	return syscall(SYS_gettid);
}
void removeLastthread(void){
	_threadCount--;
	int currentThread = _threadCount / 20;
	int locationOfThread = _threadCount % 20;
	free(threads[currentThread][locationOfThread]);
}
struct thread *addNewThread(void *(*start_routine)(void *, void *args)){
	int currentThread = _threadCount / 20;
	int locationOfThread = _threadCount % 20;
	if(!threads[currentThread]){
		threads[currentThread] = (struct thread **)malloc(sizeof(struct thread *) * 16);
	}
	threads[currentThread] = (struct thread *)malloc(sizeof(struct thread));
	threads[currentThread][locationOfThread] -> foo = foo;
	threads[currentThread][locationOfThread] -> args = args;
	threads[currentThread][locationOfThread] -> return_value = NULL;
	threads[currentThread][locationOfThread] -> state = threadNotStarted;
	threads[currentThread][locationOfThread] -> stack = (char *)malloc(stack);
	threadCount++;

	return threads[currentThread][locationOfThread];
}
int wrapper(void *myCurrentThread){
	((struct thread *)myCurrentThread) -> return_value = ((struct thread *)myCurrentThread) ->foo(((struct thread *)myCurrentThread) -> args);
	((struct thread *)myCurrentThread) -> state = threadTerminated;
	return 0;
}
int mythread_create(thread_t *myThread, void *(*foo)(void *), void *args) {
	int status;
	struct thread *t;
	t = addNewThread(foo, args);
	*mythread = _threadCount;
	if(!t){
		printf("Error occured while creating thread t");
		return(-1);
	}
	t -> state = threadRunning;
	status = clone(wrapper, (void *)(t -> stack + stack), SIGCHLD | CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES, (void *)t);
	if(status == -1){
		removeLastthread();
		return -1;
	}
	else{
		*mythread = t -> tid;
	}
	return 0;
}
thread_t mythread_self(void)
{
	pid_t pid = getpid()
	int init, i;
	mythread_lock();
	superlock_unlock();
	if(init == 0)
		return -1;
	for(i = 0; i < init; i++)
	{
		cur = init / 20;
		localinit = init % 20;
		if(pid == thread[currentThread][locationOfThread] -> tid)
			return i + 1;
	}
	return 0;
	}
}
int mythread_create(thread_t *mythread, const thread_attr *attr, void *(*start_routine)(void *), void *args)
{
    thread_t tid_t;
	tid_t = gettid();
	if(tid == getpid())
	{
        return NULL;
    }
	mythread *t = (mythread *)thread;
	mythread -> start_routine = mythread -> args = NULL;
	mythread -> tid_t = gettid();
	mythread_spin_init(&lock);
	thread = addNewThread(start_routine, args);
	if (start_routine = NULL)
	{
		mythread_unlock(&lock);
		return EINVAL;
	}
	mythread *thread = (mythread *) calloc(1, sizeof(mythread));
	if (thread == NULL)
	{
		mythread_unlock(&lock);
		return EAGAIN;
	}
	thread -> start_routine = start_routine;
	thread -> args = args;
	thread -> stack = stack;
	if (t -> stack = NULL)
	{
		mythread_unlock(&lock);
		return ENOMEM;
	}
	else{
		t -> tid = status;
		mythread_unlock(&lock);
		return 0;
		}
}
int mythread_join(thread_t mythread, void **return_value)
{
	mythread_lock(&lock);
	mythread--;
	currentThread = mythread/20;
	locationOfThread = mythread%20;
	if (mythread == NULL)
	{
		mythread[currentThread][locationOfThread] -> tpid = getpid();
		mythread_unlock(&lock);
		return ESRCH;
		while(1){
			if(return_value){
				*return_value = mythread[currentThread][locationOfThread]->return_value;
				status = 0;
				break;
		}
	}
	if (mythread -> state = threadJoin || mythread -> state = DETACHED)
	{
		mythread_unlock(&lock);
		status = EINVAL;
		break;
	}
	default:
		mythread_unlock(&lock);
		status = EINVAL;
		break;
		}
	return status;
}
int mythread_spin_init(thread_spinlock_t *lock, int pshared)
{
	*lock = 0;
	return mythread_spin_init(&lock, pshared);
}
inline int mythread_lock(thread_spinlock_t *lock);
{
	if (*lock != 0 || *lock != 1)
	{
		return EINVAL;
	}
	else
	{
		while(thread_spinlock_t(*lock))
		return 0;
	}
}
int mythread_unlock(thread_spinlock_t *lock);
{
	if(*lock != 1)
		return EINVAL;
	else if ((*lock) != thread)
	{
		return EACCESS;
	}
	else
	{
		*lock = 0;
		return 0;
	}
}
int mythread_mutex_lock(thread_mutex_t *mutex);
{
	mythread thread;
	if (!(mutex) || !(*mutex))
	{
		return EINVAL;
	}
	while(*mutex)
	{
		wait(*mutex);

	}
	return 0;
}
int mythread_mutex_unlock(thread_mutex_t *mutex);
{
	if (*mutex != 1)
	{
		return EINVAL;
	}
	thread = mythread_self();
	while(!(*mutex))
	{
		state = thread -> wait;
		wait(*mutex);
		state = thread -> threadRunning;
		status(*mutex, thread);
		return 0;
	}
}
int mythread_kill(thread_t thread, int signum);
{
	if(signum == 0)
	{
		return 0;
	}
	int status;
	mythread--;
	currentThread = mythread / 20;
	locationOfThread = mythread % 20;
	if (signum == SIGCONT || signum == SIGTERM || signum == SIGSTOP)
	{
		pid_t tgid = getpid();
		int err = kill(tgid, thread, signum);
		status = kill(thread[currentThread][locationOfThread] -> tid, signum);
		return status;
		}
    else if (err == -1)
        return errno;
    return 0;
}





