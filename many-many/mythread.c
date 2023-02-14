
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
static ucontext_t contextMainFunc;				//context of main thread

static int totalThreads = 0, indexOfCurrThread = 0;	
static activeThreads *active = NULL, *mainthread = NULL, *previous = NULL, *end = NULL;
									

threadSpinlock lock = 0;
		
static sighandler_t defaultSigHandlers[32], mainThreadSigHandlers[32], sigDfls[32];
static void addsignal(pendingSignalsQueue *q, int sig) {
	if(q->head) {
		q->tail->next = (struct pendingSignalsNode *)malloc(sizeof(struct pendingSignalsNode));
		q->tail->next->sig = sig;
		q->tail->next->next = NULL;
		q->tail = q->tail->next;
	}
	else {
		q->head = q->tail = (struct pendingSignalsNode *)malloc(sizeof(struct pendingSignalsNode));
		q->head->sig = sig;
		q->head->next = NULL;
	}
}


static void signalHandler(int sig) {
	int thread, cur, locind;
	thread = active->thread - 1;
	cur = thread / 20;
	locind = thread % 20;
	if(thread == -1) {
		if(mainThreadSigHandlers[sig] == SIG_DFL)
			sigDfls[sig](sig);
		else
			mainThreadSigHandlers[sig](sig);
	}
	else {
		if(threadsArr[cur][locind]->handlers[sig] == SIG_DFL)
			sigDfls[sig](sig);
		else
			threadsArr[cur][locind]->handlers[sig](sig);
	}
}
