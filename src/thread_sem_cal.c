#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


/*
 *	一个线程负责计算结果
 *	一个线程负责获取结果
 */

typedef struct
{
	int		res;
	sem_t	sem;
	
}Result;

//计算并将结果放置在Result中的线程运行函数
void * set_fn(void *arg)
{
	Result *r = (Result *)arg;

	int i = 1, sum = 0;
	for (; i <= 100; i++)
	{
		sum += i;
	}

	//将计算的结果放置到Result的res中
	r->res = sum;


	//V(1)操作
	sem_post(&r->sem);


	return (void *)0;
}

//获取结果的线程运行函数
void * get_fn(void *arg)
{
	Result *r = (Result *)arg;

	//P(1)操作
	sem_wait(&r->sem);

	//获取计算的结果
	int res = r->res;

	printf ("0x%lx get sum is %d\n", pthread_self(), res);

	return (void *)0;
}

int main(int argc, char **argv)
{
	int err;
	pthread_t	cal, get;

	Result r;
	sem_init(&r.sem, 0, 0);

	//启动获取结果的线程
	err = pthread_create(&get, NULL, get_fn, (void *)&r);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	//启动计算结果的线程
	err = pthread_create(&cal, NULL, set_fn, (void *)&r);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	pthread_join(cal, NULL);
	pthread_join(get, NULL);

	return 0;
}

