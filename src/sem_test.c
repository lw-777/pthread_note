#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


//定义两个线程信号量
sem_t sem1;
sem_t sem2;

void *a_fn(void *arg)
{
	sem_wait(&sem1);
	printf ("Thread a is running\n");

	return (void *)0;
}

void *b_fn(void *arg)
{
	sem_wait(&sem2);
	printf ("Thread b is running\n");
	
	//释放线程a(对线程信号量sem1做加1操作，
	//让阻塞的线程a继续运行）
	sem_post(&sem1);
	return (void *)0;
}

void *c_fn(void *arg)
{	
	printf ("Thread c is running\n");

	//释放线程b(对线程信号量sem2做加1操作，
	//让阻塞的线程b继续运行）
	sem_post(&sem2);
	return (void *)0;
}


int main(void)
{
	int err;
	pthread_t	a, b, c;

	//线程信号量初始化，初始值为0
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);

	err = pthread_create(&a, NULL, a_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	err = pthread_create(&b, NULL, b_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	err = pthread_create(&c, NULL, c_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	pthread_join(a, NULL);
	pthread_join(b, NULL);
	pthread_join(c, NULL);

	//销毁信号量
	sem_destroy(&sem1);
	sem_destroy(&sem2);

	return 0;
}

