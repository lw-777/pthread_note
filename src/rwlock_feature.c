#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//定义读写锁
pthread_rwlock_t rwlock;

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf ("-usage:%s [r|w] [r|w]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//读写锁的初始化
	pthread_rwlock_init(&rwlock, NULL);

	if (!strcmp(argv[1], "r"))
	{
		//加读锁
		if (pthread_rwlock_rdlock(&rwlock) != 0)
		{
			printf ("first read lock failure!\n");
		}
		else
		{
			printf ("first read lock success!\n");
		}
	}
	else if (!strcmp(argv[1], "w"))
	{
		//加写锁
		if (pthread_rwlock_wrlock(&rwlock) != 0)
		{
			printf ("first write lock failure!\n");
		}
		else
		{
			printf ("first write lock success!\n");
		}
	}

	if (!strcmp(argv[2], "w"))
	{
		//加写锁
		if (pthread_rwlock_wrlock(&rwlock) != 0)
		{
			printf ("second write lock failure!\n");
		}
		else
		{
			printf ("second write lock success!\n");
		}
	}
	else if (!strcmp(argv[2], "r"))
	{
		//加读锁
		if (pthread_rwlock_rdlock(&rwlock) != 0)
		{
			printf ("second read lock failure!\n");
		}
		else
		{
			printf ("second read lock success!\n");
		}
	}

	//释放读写锁
	pthread_rwlock_unlock(&rwlock);
	pthread_rwlock_unlock(&rwlock);


	return 0;
}

