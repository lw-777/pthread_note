#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
	long d1;
	long d2;
}Arg;

void * th_fn(void *arg)
{
	Arg *r = (Arg *)arg;

	return (void *)r;

}

int main(void)
{
	int err;
	pthread_t th;
	Arg t = {20, 50};

	err = pthread_create(&th, NULL, th_fn, (void *)&t);
	if (err != 0)
	{
		perror ("pthread_create error!");
	}
	
	//用long的原因：消除指针和int类型内存大小不匹配的警告
	/*
#if 0
	long *ret;
	pthread_join(th, (void **)&ret);
#else
	long ret;
	pthread_join(th, (void *)&ret);
#endif
*/
	long *result;
	pthread_join(th, (void **)&result);

	printf ("result is %ld\n", ((Arg* )result)->d1 + ((Arg* )result)->d2);

	return 0;
}

