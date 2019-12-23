#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

extern int usleep();

typedef struct
{
	char name[20];
	int time;
	long start;
	long end;

}Arg;

//定义线程执行的开始函数
void *th_fn(void *arg)
{
	Arg *r = (Arg *)arg;
	int i = r->start;

	for (; i <= r->end; i++)
	{
		printf ("%lx run %d\n", pthread_self(), i);
		usleep(r->time);	//微秒

	}

	//return (void *)(int )0;
	return (void *)(r->end - r->start);
}

int main(void)
{

	int err;
	pthread_t rabbit, turtle;	//定义两个线程标识符

	Arg r_a = 
	{"rabbit", (int )(drand48() * 100000000), 20, 50};

	Arg t_a = 
	{"turtle", (int )(drand48() * 100000000), 10, 60};

	//创建rabbit线程
	err = pthread_create(&rabbit, NULL, th_fn, (void *)&r_a);

	if (err != 0)
	{
		perror("pthread_create error!");
	}

	//创建turtle线程
	err = pthread_create(&turtle, NULL, th_fn, (void *)&t_a);

	if (err != 0)
	{
		perror("pthread_create error!");
	}

	/*
	//主控线程条用pthread_join(), 自己会阻塞
	//直到rabbit和turtle线程运行结束才能继续运行
	pthread_join(rabbit, NULL);
	pthread_join(turtle, NULL);
	*/

	long result;
	pthread_join(rabbit, (void *)&result);
	printf ("rabbit distance is %ld\n", result);

	pthread_join(turtle, (void *)&result);
	printf ("turtle distance is %ld\n", result);


	return 0;
}


