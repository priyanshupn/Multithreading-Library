/*
Tests thread kill
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mythread.h"
#include <signal.h>
#include <unistd.h>
void *func()
{
    printf("%s", "hi");
    int i = 1;
    while (i)
    {
        printf("%d", i);
        i++;
    }
}

int main()
{
    void *status;
    threadPtr thread1, thread2;
    mythreadInit();
    mythreadCreate(&thread1, func, NULL);
    mythreadCreate(&thread2, func, NULL);
    sleep(1);
    mythreadKill(thread1, SIGINT);
    mythreadKill(thread2, SIGSTOP);
    return 0;
}
