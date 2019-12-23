#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int sleep();

void out_state(pthread_attr_t *attr)
{
	int state;
	if (pthread_attr_getdetachstate(attr, &state) != 0)
	{
		perror ("getdetachstate error!");
	}
	else
	{
		if (state == PTHREAD_CREATE_JOINABLE)
		{
			printf ("joinable state\n");
		}
		else if (state == PTHREAD_CREATE_DETACHED)
		{
			printf ("detached state\n");
		}
		else
		{
			printf ("error state\n");
		}
	}
}	

void * th_fn(void *arg)
{
	long i;
	long sum = 0;

	for (i = 1; i <= 100; i++)
	{
		sum += i;
	}

	return (void *)sum;
}

int main(void)
{

	int err;
	pthread_t default_th, detach_th;

	//定义线程属性
	pthread_attr_t attr;
	//对线程属性初始化
	pthread_attr_init(&attr);
	//输出分离属性
	out_state(&attr);

	//取分离属性的默认值，以正常方式启动的子线程
	err = pthread_create(&default_th, &attr, th_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	long ret;
	err = pthread_join(default_th, (void *)&ret);
	if (err != 0)
	{
		perror ("pthread_join error!");
	}
	else
	{
		printf ("default return is %ld\n", (long )ret);
	}

	printf ("--------------------------------------------\n");

	//设置分离属性
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	out_state(&attr);

	//以分离状态启动子线程
	err = pthread_create(&detach_th, &attr, th_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	/*
	err = pthread_join(detach_th, (void *)&ret);
	if (err != 0)
	{
		//perror ("pthread_join error!");
		fprintf (stderr, "%s\n", strerror(err));
	}
	else
	{
		printf ("default return is %ld\n", (long )ret);
	}
	*/

	//销毁线程属性
	pthread_attr_destroy(&attr);

	printf ("0x%lx finished\n", pthread_self());
	sleep(1);

	return 0;
}


