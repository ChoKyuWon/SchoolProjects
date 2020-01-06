#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

void *thread_function(void *t_name){
    pid_t pid; // process id
    pthread_t tid; // thread id
    pid = getpid();
    tid = pthread_self();
    char* thread_name = (char*)t_name;
    int i=0;
    for(i=0;i<3;i++){
        printf("[%s] pid:%d tid:%x\n", thread_name, pid, tid);
    }
}

int main(){
    pthread_t p_thread[2];
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    int thr_id;
    int status;
    char p_1[]="thread_1";
    char p_2[]="thread_2";
    char p_M[]="thread_m";

    sleep(1);
    thr_id=pthread_create(&p_thread[0],NULL,thread_function,(void *)p_1);
    if(thr_id<0){
        printf("create fail!\n");
        exit(0);
    }

    thr_id=pthread_create(&p_thread[1],NULL,thread_function,(void *)p_2);
    if(thr_id<0){
        printf("create fail!\n");
        exit(0);
    }

    for(int i=0;i<3;i++){
        printf("[%s] pid:%d tid:%ld\n", p_M, pid, tid);
    }
    //main() function also execute function
    pthread_join(p_thread[0],NULL);
    pthread_join(p_thread[1],NULL);
  // wait for child process thread 
}