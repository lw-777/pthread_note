#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void * th_fn(void *arg)
{
	long a = (long )arg;

	printf ("%ld\n", a);

	return (void *)0;

}

int main(void)
{
	int err;
	pthread_t th1, th2;

	err = pthread_create(&th1, NULL, th_fn, (void *)0);
	if (err != 0)
	{
		perror("pthread_create error!");
	}

	pthread_join(th1, NULL);

	err = pthread_create(&th2, NULL, th_fn, (void *)1);
	if (err != 0)
	{
		perror("pthread_create error!");
	}
	pthread_join(th2, NULL);


	return 0;
}


