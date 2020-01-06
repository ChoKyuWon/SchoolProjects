/* * hello.c - Pthreads "hello, world" program */
#include "pthread.h"
#include<stdio.h>
#include<stdlib.h>

int global_var=0;

void *thread(void *vargp);
int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    printf("%d\n",global_var);
    pthread_join(tid, NULL);
    exit(0);
}
/* thread routine */
void *thread(void *vargp){
    global_var=100;
    printf("Hello, world!\n");
    return NULL;
}
