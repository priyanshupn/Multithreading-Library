#include <bits/types.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#define _GNU_SOURCE
#define stack 1024*1024
enum state {threadRunning, threadNotStarted, threadTerminated, threadJoined, threadCollected};
typedef unsigned int thread_t;
struct mythread_struct *t
typedef unsigned long int thread_attr;
typedef unsigned int thread_spinlock_t;
typedef unsigned int thread_mutex_t;
struct thread
{
    int tid_t, state;
    pid_t tpid;
    char *stack;
    void *(*start_routine)(void *);
    void *args;
    void *return_value;
};
int mythread_create(thread_t *mythread, const thread_attr *attr, void *(*start_routine)(void *), void *args);
int mythread_join(thread_t mythread, void **return_value);
{
    int cur, local, status, cstatus;
}
void mythread_exit(void *return_value);
int mythread_lock(thread_spinlock_t *lock);
int mythread_unlock(thread_spinlock_t *lock);
int mythread_mutex_lock(thread_mutex_t *mutex);
int mythread_mutex_unlock(thread_mutex_t *mutex);
int mythread_kill(thread_t thread, int sig);
