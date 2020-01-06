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
char str[10];
int idx;

void *thread(void *arg){
	int i;
	int c = (int)arg;

	if(mutex_lock(&m)){
		printf(1, "mutex lock failed\n");
		exit();
	}
		
	printf(1, "arg: %d, idx: %d\n", c, idx);

	str[idx] = c;

	for(i=0;i<10000000;i++);

	idx = idx+1;

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

	printf(1, "TEST1: ");

	for(i=0;i<NTHREAD;i++)
		stack[i] = malloc(4096);

	if(mutex_lock(&m) != -2){
		printf(1, "non-initialized mutex lock\n");
		exit();
	}
	int res = mutex_unlock(&m);
	if(res != -2){
		printf(1, "non-initialized mutex unlock: %d\n");
		exit();
	}

	if(mutex_init(&m)){
		printf(1, "mutex init failed\n");
		exit();
	}

	if(mutex_init(&m) != -2){
		printf(1, "mutex reinitializing\n");
		exit();
	}

	if(mutex_lock(&m)){
		printf(1, "mutex lock failed\n");
		exit();
	}

	if(mutex_lock(&m) != -3){
		printf(1, "mutex lock again\n");
		exit();
	}
	
	for(i=0;i<NTHREAD-1;i++){
		tid[i] = thread_create(thread, (void *)('a'+i), stack[i]);

		if(tid[i] == -1){
			printf(1, "thread create failed\n");
			exit();
		}
	}

	if(mutex_unlock(&m)){
		printf(1, "mutex unlock failed\n");
		exit();
	}

	if(mutex_unlock(&m) != -3){
		printf(1, "non-owned mutex unlock\n");
		exit();
	}

	for(i=0;i<NTHREAD-1;i++){
		if(thread_join(tid[i], &retval[i]) == -1){
			printf(1, "thread join failed\n");
			exit();
		}
	}
			
	printf(2, "str: %s\n", str);

	if(strcmp(str, "abcdefg") != 0){
		printf(1, "strcmp failed\n");
		exit();
	}

	for(i=0;i<NTHREAD;i++)
		free(stack[i]);

	printf(1, "OK\n");

	exit();
}