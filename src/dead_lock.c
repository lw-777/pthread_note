#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern int sleep();

typedef struct
{
	int value;
	pthread_mutex_t mutex;
}ResourceA;

typedef struct
{
	int value;
	pthread_mutex_t mutex;
}ResourceB;

typedef struct
{
	ResourceA *ra;
	ResourceB *rb;
}Storage;

void * a_fun(void *arg)
{
	Storage *s = (Storage *)arg;
	
	//对ResourceA加锁
	pthread_mutex_lock(&s->ra->mutex);
	sleep(1);
	printf ("0x%lx is waiting for ResourceB...\n", pthread_self());
	
	//对ResourceB加锁
	pthread_mutex_lock(&s->rb->mutex);
	printf ("ResourceA value is : %d\n", s->ra->value);
	printf ("ResourceB value is : %d\n", s->rb->value);
	pthread_mutex_unlock(&s->ra->mutex);
	pthread_mutex_unlock(&s->rb->mutex);
	
	return (void *)0;
}

void *b_fun(void *arg)
{
	Storage *s = (Storage *)arg;

#if 0
	//对ResourceB加锁
	pthread_mutex_lock(&s->rb->mutex);
	
	sleep(1);
	printf ("0x%lx is waiting for ResourceA...\n", pthread_self());
	
	//对ResourceA加锁
	pthread_mutex_lock(&s->ra->mutex);

	printf ("ResourceA value is : %d\n", s->ra->value);
	printf ("ResourceB value is : %d\n", s->rb->value);

	pthread_mutex_unlock(&s->rb->mutex);
	pthread_mutex_unlock(&s->ra->mutex);
#else
	//对ResourceA加锁
	pthread_mutex_lock(&s->ra->mutex);
	
	sleep(1);
	printf ("0x%lx is waiting for ResourceB...\n", pthread_self());
	
	//对ResourceB加锁
	pthread_mutex_lock(&s->rb->mutex);

	printf ("ResourceA value is : %d\n", s->ra->value);
	printf ("ResourceB value is : %d\n", s->rb->value);

	pthread_mutex_unlock(&s->ra->mutex);
	pthread_mutex_unlock(&s->rb->mutex);
#endif

	return (void *)0;
}


int main(void)
{
	ResourceA ra;
	ResourceB rb;

	ra.value = 100;
	rb.value = 200;

	pthread_mutex_init(&ra.mutex, NULL);
	pthread_mutex_init(&rb.mutex, NULL);

	Storage s = {&ra, &rb};

	int err;
	pthread_t thread_a, thread_b;

	err = pthread_create(&thread_a, NULL, a_fun, (void *)&s);
	if (err != 0)
	{
		perror("pthread_create error!");
		exit (EXIT_FAILURE);
	}

	err = pthread_create(&thread_b, NULL, b_fun, (void *)&s);
	if (err != 0)
	{
		perror("pthread_create error!");
		exit(EXIT_FAILURE);
	}

	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);

	pthread_mutex_destroy(&ra.mutex);
	pthread_mutex_destroy(&rb.mutex);

	return 0;
}

