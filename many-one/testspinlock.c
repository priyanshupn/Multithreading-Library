
/*
Tests spinLock 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mythread.h"
static int glob = 0;
static mythread_lock locking = 0;
 
static void * Func(void *arg)
{
    for (int j = 0; j < 10; j++) {
      
       mythread_lock(&locking);
            

        for (int k = 0; k < 10; k++)
            glob++;

       
       mythread_unlock(&locking);
    }

    return NULL;
}

int
main(int argc, char *argv[])
{

    int totalThreads;
    threadPtr *thread;
         
    totalThreads = atoi(argv[1]);
    thread = calloc(totalThreads, sizeof(threadPtr));
    mythread_init();
    if (thread == NULL)
        printf("calloc");
    
    for (int j = 0; j < totalThreads; j++) {
        mythread_create(&thread[j], Func, NULL);
    }

    for (int j = 0; j < totalThreads; j++) {
        mythread_join(thread[j], NULL);
    }
    printf("glob = %d\n", glob);
    
}
