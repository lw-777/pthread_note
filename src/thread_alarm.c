#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

extern int sleep();
extern int alarm();

void sig_handler(int signo)
{
	printf ("pthread id is the sig_handler : %lx\n", pthread_self());

	if (signo == SIGALRM)
	{
		printf ("timeout...\n");
	}
	alarm(2);
}


void *th_fn(void *arg)
{
	if (signal(SIGALRM, sig_handler) == SIG_ERR)
	{
		perror ("signal error!\n");
		exit(EXIT_FAILURE);
	}
	//在子线程中设置定时器
	alarm(2);

	int i;
	for (i = 1; i <= 100; i++)
	{
		printf ("(%lx) i : %d\n", pthread_self(), i);
		sleep(1);
	}

	return (void *)0;	//pthread_exit((void *)0);
}

void *th_fn2(void *arg)
{
	pthread_t th1 = (pthread_t)arg;

	int i;
	for (i = 1; i <= 15; i++)
	{
		if (i == 5)
		{
			//终止线程1的运行
			pthread_cancel(th1);
			alarm(0);
		}
		printf ("(%lx) i : %d\n", pthread_self(), i);
		sleep(1);
	}
	
	return (void *)0;
}

int main(void)
{
	int err;
	pthread_t	th, th2;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	err = pthread_create(&th, NULL, th_fn, (void *)0);
	if (err != 0)
	{
		perror("pthread_create error!");
		exit(EXIT_FAILURE);
	}

	err = pthread_create(&th2, NULL, th_fn2, (void *)th);
	if (err != 0)
	{
		perror("pthread_create error!");
		exit(EXIT_FAILURE);
	}

	sigset_t	set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);

	//对主控线程屏蔽SIGALRM信号
	pthread_sigmask(SIG_SETMASK, &set, NULL);

	while (1)
	{
		printf ("control thread(%lx) is running\n", pthread_self());
		sleep(10);
	}

	printf ("control thread is over\n");

	return 0;
}

