#include "types.h"
#include "stat.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"
#include "user.h"
#include "synch.h"

#define NTHREAD 8

void *stack[NTHREAD];
int tid[NTHREAD];
void *retval[NTHREAD];

struct mutex_t m;
struct cond_t c;
int buffer[10];
int idx=0;
int condition = 0;

void *thread(void *arg){
	if(mutex_lock(&m)){
		printf(1, "mutex lock failed\n");
		exit();
	}

	while(condition == 0){
		if(cond_wait(&c, &m)){
			printf(1, "cond wait failed\n");
			exit();
		}
	}

	condition--;
	buffer[idx++] = (int)arg;

	if(mutex_unlock(&m)){
		printf(1, "mutex unlock failed\n");
		exit();
	}

	thread_exit(0);
}

int
main(int argc, char **argv)
{
	int i;

	printf(1, "TEST2: ");

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);
	if(mutex_init(&m)){
		printf(1, "mutex init failed\n");
		exit();
	}
	if(cond_wait(&c, &m) != -2){
		printf(1, "non-initialized cond wait\n");
		exit();
	}
	if(cond_signal(&c) != -2){
		printf(1, "non-initialized cond signal\n");
		exit();
	}
	if(cond_init(&c)){
		printf(1, "cond init failed\n");
		exit();
	}
	if(cond_init(&c) != -2){
		printf(1, "reinitialize cond init\n");
		exit();
	}
	if(cond_wait(&c, &m) != -3){
		printf(1, "non-owned mutex\n");
		exit();
	}
	for(i=0;i<NTHREAD-1;i++){
		tid[i] = thread_create(thread, (void *)i, stack[i]);

		if(tid[i] == -1){
			printf(1, "thread create failed\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD-1;i++){
		if(mutex_lock(&m)){
			printf(1, "mutex lock failed\n");
			exit();
		}
		condition++;
		if(cond_signal(&c)){
			printf(1, "cond signal failed\n");
			exit();
		}
		if(mutex_unlock(&m)){
			printf(1, "mutex unlock failed\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD-1;i++){
		if(thread_join(tid[i], &retval[i]) == -1){
			printf(1, "thread join failed\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD-1;i++){
		if(buffer[i] != i){
			printf(1, "WRONG\n");
			exit();
		}
	}

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	printf(1, "OK\n");

	exit();
}