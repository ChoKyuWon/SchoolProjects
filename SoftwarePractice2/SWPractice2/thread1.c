#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int sum;
void *sumf(void *value){
    int i, upper = strtol(value, 0, 0); // string to long
    for(int i=1;i<=upper;i++){
        sum+=i;
    }
    //calculate the number and exit the thread

}
int main(int argc, char *argv[]){
    pthread_t tid;

    if(argc < 2){
        printf("Usage: %s number\n", argv[0]);
        exit(-1);
    }

    pthread_create(&tid, NULL, (void*)sumf, (void*)argv[1]);
    pthread_join(tid,NULL);
    
    printf("sum = %d\n",sum);
}