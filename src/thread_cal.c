#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

extern int usleep();
extern int sleep();

/*
 *	一个线程负责计算结果
 *	一个线程负责获取结果
 */

typedef struct
{
	int				res;
	int				is_wait;	//用户给出的判断条件
	pthread_cond_t	cond;		//条件变量
	pthread_mutex_t	mutex;		//互斥锁
	
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

	//对两个线程共享的判断条件进行保护
	pthread_mutex_lock(&r->mutex);

	//判断获取结果的线程是否准备好
	while (!r->is_wait)
	{
		pthread_mutex_unlock(&r->mutex);
		usleep(100);
		pthread_mutex_lock(&r->mutex);
	}
	pthread_mutex_unlock(&r->mutex);

	//通知唤醒等待的那个获取结果的线程
	pthread_cond_broadcast(&r->cond);

	return (void *)0;
}

//获取结果的线程运行函数
void * get_fn(void *arg)
{
	Result *r = (Result *)arg;
	//对两个线程共享的判断条件进行保护(加锁)
	//两个线程对判断条件的操作是互斥的
	pthread_mutex_lock(&r->mutex);

	//表示获取结果的线程已经准备就绪
	r->is_wait = 1;

	//获取结果的线程进行等待
	//为什么等待要在释放锁之前？？？
	pthread_cond_wait(&r->cond, &r->mutex);
	pthread_mutex_unlock(&r->mutex);

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
	r.is_wait = 0;
	pthread_cond_init(&r.cond, NULL);
	pthread_mutex_init(&r.mutex, NULL);

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

	pthread_cond_destroy(&r.cond);
	pthread_mutex_destroy(&r.mutex);

	return 0;
}

