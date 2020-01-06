#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define QSIZE 10
#define LOOP 50

typedef struct{
	int data[QSIZE];
	int index;
	int count;
	pthread_mutex_t lock;
	pthread_cond_t notfull;
	pthread_cond_t notempty;
} queue_t;

queue_t *qinit();
void qdelete(queue_t *q);
void *produce(void *args);
void *consume (void *args);
void put_data(queue_t *q, int d);
int get_data(queue_t *q);

int seed;

int main()
{
	queue_t *q;
	pthread_t producer, consumer;

	seed = time(NULL);

	q = qinit();

	pthread_create(&consumer, NULL, consume, (void *)q);
	pthread_create(&producer, NULL, produce, (void *)q);
	

	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);

	qdelete(q);
}

queue_t *qinit()
{
	queue_t *q;
	q = (queue_t *)malloc(sizeof(queue_t));
	q->index = q->count = 0;
	
	//Dynamic mutex initialization(lock, full, empty)
	//OK
	pthread_mutex_init (&q->lock, (pthread_mutexattr_t *)NULL);
	pthread_cond_init (&q->notempty, (pthread_condattr_t *)NULL);
	pthread_cond_init (&q->notfull, (pthread_condattr_t *)NULL);

	return q;
}

void qdelete(queue_t *q)
{
	//destroy mutex & condition
	//OK
	printf("qdelete!\n");
	pthread_mutex_destroy (&(q->lock));
	pthread_cond_destroy(&(q->notempty));
	pthread_cond_destroy(&(q->notfull));
	//free queue
	//OK
	free(q);

}

void *produce(void *args)
{
	int i, d;
	queue_t *q = (queue_t *)args;
	for(i=0;i<LOOP;i++)
	{
		d = rand_r(&seed) % 10;
		printf("put data %d to queue\n", d);
		put_data(q, d);
		printf("put data %d to queue\n", d);
	}
	pthread_exit(NULL);
}

void *consume(void *args)
{
	int i, d;
	queue_t *q = (queue_t *)args;
	for(i=0;i<LOOP;i++)
	{
		d = get_data(q);
		printf("got data %d from queue\n", d);
	}
	pthread_exit(NULL);
}

void put_data(queue_t *q, int d)
{
	//lock
	//OK
	pthread_mutex_lock(&q->lock);
	printf("put : mutex is locked!\n");


	//conditional wait (when q is full)
	//pthread_cond_wait() : to block on a condition variable
	//atomically release mutex and cause the calling thread to block on the condition variable
	//access by another thread to the mutex and then the condition variable
	//acqure the mutex after the about-to-block thread has released it

	//OK...?
	printf("%d is q size\n", q->count);
	if(q->count==QSIZE){
		/*q가 가득 차 있을때*/
		pthread_cond_wait(&q->notfull,&q->lock);
	}

	q->data[(q->index + q->count) % QSIZE] = d;
	q->count++;


	//signal
	pthread_cond_signal (&q->notempty);
	
	//unlock
	//OK
	printf("put : mutex is unlocked!\n");
	pthread_mutex_unlock(&q->lock);
}

int get_data(queue_t *q)
{
	int d;
	//lock
	pthread_mutex_lock(&q->lock);
	printf("got : mutex is locked!\n");
	//condition wait (when q is empty)
	if(q->count==0)
		//wait를 하면 lock이 풀림!
		pthread_cond_wait(&q->notempty,&q->lock);

	d = q->data[q->index];
	q->index = (q->index + 1) % QSIZE;
	q->count--;

	//signal
	pthread_cond_signal(&q->notfull);

	//unlock
	pthread_mutex_unlock(&q->lock);
	printf("got : mutex is unlocked!\n");

	return d;
}


