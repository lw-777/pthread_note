#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

extern int usleep();

//定义线程执行的开始函数
void *th_fn(void *arg)
{
	long distance = (long )(void *)arg;
	int i;

	for (i = 1; i <= distance; i++)
	{
		printf ("%lx run %d\n", pthread_self(), i);
		int time = (int )(drand48() * 100000);

		usleep(time);	//微秒

	}

	return (void *)(int )0;
}

int main(void)
{

	int err;
	pthread_t rabbit, turtle;	//定义两个线程标识符

	//创建rabbit线程
	err = pthread_create(&rabbit, NULL, th_fn, (void *)50);

	if (err != 0)
	{
		perror("pthread_create error!");
	}

	//创建turtle线程
	err = pthread_create(&turtle, NULL, th_fn, (void *)50);

	if (err != 0)
	{
		perror("pthread_create error!");
	}

	//主控线程条用pthread_join(), 自己会阻塞
	//直到rabbit和turtle线程运行结束才能继续运行
	pthread_join(rabbit, NULL);
	pthread_join(turtle, NULL);

	printf ("control thread id: %lx\n", pthread_self());
	printf ("finished!\n");


	return 0;
}


