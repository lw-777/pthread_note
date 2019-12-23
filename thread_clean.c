#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//定义清理函数
void clean_fun(void *arg)
{
	char *s = (char *)arg;
	printf ("clean_function:%s\n", s);
}

void * th_fn(void *arg)
{
	long execute = (long )arg;
	pthread_cleanup_push(clean_fun, (void *)"first clean function");
	pthread_cleanup_push(clean_fun, (void *)"second clean function");

	printf ("thread running %lx\n", pthread_self());

	pthread_cleanup_pop(execute);
	pthread_cleanup_pop(execute);

	return (void *)0;

}

int main(void)
{
	int err;
	pthread_t th1, th2;

	err = pthread_create(&th1, NULL, th_fn, (void *)0);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	pthread_join(th1, NULL);
	printf ("th1(%lx) finished\n", th1);
	
	err = pthread_create(&th2, NULL, th_fn, (void *)1);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}

	pthread_join(th2, NULL);
	printf ("th2(%lx) finished\n", th2);


	return 0;
}

