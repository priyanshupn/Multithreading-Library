#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <ucontext.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include "mythread.h"
//a 2d array of pointers which will store addresses of all threads
mythread **threadsArr[20] = {0};
static ucontext_t context;
static int totalThreads = 0, indexOfCurrThread = 0;	
static activeThreads *active = NULL, *mainthread = NULL, *previous = NULL, *end = NULL;
thread_spinlock_t = 0;
static void add(pending_signals *q, int sig) {
	if(q->head) {
		q->tail->next = (struct pending_signals *)malloc(sizeof(struct pending_signals));
		q->tail->next->sig = sig;
		q->tail->next->next = NULL;
		q->tail = q->tail->next;
	}
    else {
		q->head = q->tail = (struct pending_signals *)malloc(sizeof(struct pending_signals));
		q->head->sig = sig;
		q->head->next = NULL;
	}
}
static void signal_handler(int sig) 
{
	int thread, cur, locationofthread;
	thread = active -> thread - 1;
	cur = thread / 20;
	locationofthread = thread % 20;
	if(thread == -1) 
	{
		if(signal_handler[sig] == SIG_DFL)
			sigDfls[sig](sig);
		else
			signal_handler[sig](sig);
	}
	else {
		if(thread_attr[cur][locationofthread] -> signals[sig] == SIG_DFL)
			sigDfls[sig](sig);
		else
			thread_attr[cur][locationofthread] -> signals[sig](sig);
	}
}
signal_handler thread(int sigNum, signal_handler handler) {
	signal_handler *f, dflt;
	int cur, locationofthread, thread;
	if(sigNum >= 32)
		return SIG_ERR;
	mythread_lock(&lock);
	
	if(active -> thread == 0)
		f = thread;
	else {
		thread = active -> thread - 1;
		cur = thread / 20;
		locationofthread = thread % 20;
		f = thread_attr[cur][locationofthread] -> handler;
	}
	if(f[sigNum] == SIG_DFL || f[sigNum] == SIG_IGN)
		dflt = sigDfls[sigNum] = signal(sigNum, signal);
	else
		dflt = f[sigNum];
	f[sigNum] = handler;
	mythread_unlock(&lock);
	return dflt;
}
static void pending_signals()
{
	int thread, cur, locationofthread;
	thread = active -> thread - 1;
	cur = thread / 20;
	locationofthread = thread % 20;
	struct pending_signals *node, *previous;
	if(thread == -1)
		return;
	else
		node = thread_attr[cur][locationofthread] -> pending_signals.head;
	while(node)
	{
		raise(nodev-> sig);
		previous = node;
		node = node -> next;
		free(previous);
	}
	thread_attr[cur][locationofthread] -> pending_signals.head = thread_attr[cur][locationofthread]->pending_signals.tail = NULL;
}
void mythread_init()
{
	int i;
	active = (active_threads *)malloc(sizeof(active_threads));
	active -> thread = 0;
	active -> c = &context;
	active -> next = active;
	end = mainthread = active;
	indexOfCurrThread = 1;
	for(i = 0; i < 32; i++) 
		sigDfls[i] = signal_handler[i] = signals[i] = SIG_DFL;
	signal(SIGALRM, next_thread);	
}
static void next_thread(int sig) {
	if(sig == SIGALRM && indexOfCurrThread > 1 && mythread_lock(&lock)) {
		previous = active;
		active = active -> next;
		swapcontext(previous->c, active->c);
		pending_signals();
		mythread_unlock(&lock);
	}
}
mythread *addNewThread(void *(*start_routine)(void *), void *args, void **return_value)
{
	int cur = total_threads / 20;
	int locationofthread = total_threads % 20;
	if(!threadsArr[cur])
		threadsArr[cur] = (mythread **)malloc(sizeof(mythread *) * 20);
	threadsArr[cur][locationofthread] = (mythread *)malloc(sizeof(mythread));
	getcontext(&(threadsArr[cur][locationofthread]->thread_context));
	memcpy(threadsArr[cur][locationofthread]->handlers, signal_handler, sizeof(signal_handler));
	threadsArr[cur][locationofthread]->fun = fun;
	threadsArr[cur][locationofthread]->args = args;
	threadsArr[cur][locationofthread]->thread_context.uc_stack.ss_sp = malloc(STACK);
	threadsArr[cur][locationofthread]->thread_context.uc_stack.ss_size = STACK;
	threadsArr[cur][locationofthread]->thread_context.uc_link = &contextMainFunc;
	threadsArr[cur][locationofthread]->return_value = NULL;
	threadsArr[cur][locationofthread]->state = threadNotStarted;
	threadsArr[cur][locationofthread]->pendingSignals.head = threadsArr[cur][locind]->pendingSignals.tail = NULL;
	totalThreads++;
	return threadsArr[cur][locind];
}
int mythread_create(thread_t *mythread, const thread_attr *attr, void *(*start_routine)(void *), void *args)
{
	struct mythread *t; 
	active_threads *newthread;
	if(indexOfCurrThread == 1)
		ualarm(50000, 50000);
	mythread_lock(&lock);
	t = addNewThread(start_routine, args);
	*mythread = total_threads;
	if(t)
		t -> state = threadRunning;
	else {
		mythread_unlock(&lock);
		return -1;
	}
	makecontext(&(t -> thread_context), (void (*)())start_routine, 1, total_threads);
	newthread = (active_threads *)malloc(sizeof(active_threads));
	newthread -> thread = *mythread;
	newthread -> c = &(t -> thread_context);
	newthread -> next = mainthread -> next;
	mainthread -> next = newthread;
	indexOfCurrThread++;
	if(newthread -> next == mainthread)
		end = newthread;
	mythread_unlock(&lock);
	return 0;
}
thread mythread_self(void) {
	thread t;
	mythread_lock(&lock);
	if(active)
		t = active -> thread;
	else t = -1;
	mythread_unlock(&lock);
	return t;
}
int mythread_join(thread_t mythread, void **return_value);
{
	int cur, locationofthread, status = EINVAL;
	mythread--;
	cur = mythread / 20;
	locationofthread = mythread % 20;
	mythread_lock(&lock);
	if(mythread < total_threads) {
		switch(threadsArr[cur][locationofthread] -> state) {
			case threadRunning:
				threadsArr[cur][locationofthread] -> state = threadJoined;
				mythread_unlock(&lock);
				while(threadsArr[cur][locationofthread] -> state != threadTerminated)
					nextThread(SIGALRM);
				mythread_lock(&lock);
				threadsArr[cur][locationofthread] -> state = threadCollected;
				mythread_unlock(&lock);
				if(return_value)
					*return_value = threadsArr[cur][locationofthread] -> return_value;
				status = 0;
				break;
			case threadNotStarted:
			case threadCollected:
			case threadJoined:
				mythread_unlock(&lock);
				break;
			case threadTerminated:
				mythread_unlock(&lock);
				if(return_value)
					*return_value = threadsArr[cur][locationofthread] -> return_value;
				status = 0;
				break;
			default:
				mythread_unlock(&lock);
				break;
		}
	}
	return status;
}
int mythread_kill(thread_t mythread, int sig);
{	int cur, locationofthread;
	mythread--;
	cur = mythread / 20;
	locationofthread = mythread % 20;
	mythread_lock(&lock);
	add(&(threadsArr[cur][locationofthread] -> pending_signals), sig);
	mythread_unlock(&lock);
	return 0;
}
void mythread_exit(void *return_value)
{	
	int ind = active -> thread - 1;
	int cur = ind / 20, locind = ind % 20;
	ucontext_t *context;
	if(ind >= 0) {
		mythread_lock(&lock);
		context = active -> c;
		threadsArr[cur][locationofthread] -> state = threadTerminated;
		threadsArr[cur][locationofthread] -> return_value = return_value;
		if(active -> next == mainthread)
			end = previous;
		previous->next = active->next;
		free(active);
		active = mainthread;
		previous = end;
		indexOfCurrThread--;
		if(indexOfCurrThread == 1)
			ualarm(0, 0);
		swapcontext(context, &context);
		mythread_unlock(&lock);
	}
}
inline int mythread_lock(thread_spinlock_t *lock)
{
	if(*lock != 0 || *lock != 1)
		return EINVAL;
	while((lock, 1));
	return 0;
}
inline int mythread_unlock(thread_spinlock_t *unlock)
{
	if(*lockf != 1)
		return EINVAL;
	*lockf = 0;
	return 0;
}
int mythread_mutex_lock(mythread_mutex_lock *lock)
{
	while ((atomic_flag_test_and_set_explicit(&lock->value, 1)) != 0)
	{
		syscall(SYS_futex, lock->value, FUTEX_WAIT, 1, NULL, NULL, 0);
	}
	return 0;
}
int mythread_mutex_unlock(thread_mutex_t *mutex);
{
	if (atomic_fetch_sub(&lock->value, 1) != 1)
	{
		lock->value = 0;
		syscall(SYS_futex, lock->value, FUTEX_WAKE, 1, NULL, NULL, 0);
	}
	return 0;
}
