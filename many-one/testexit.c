#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "mythread.h"
#include <stdlib.h>
#include <string.h>

void *Func(void *args)
{
    mythreadExit((void *)5);
}

int main()
{
    threadPtr t1;
    void *status;
    mythreadInit();
    mythreadCreate(&t1, Func, NULL);
    mythreadJoin(t1, &status);
    printf("%d\n", (int)status);
    return 0;
}
