#include "account.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>

extern int sleep();

//创建账户
extern Account * create_account(int code, double balance)
{
	Account *a = (Account *)malloc(sizeof(Account));
	assert(NULL != a);

	a->code = code;
	a->balance = balance;
#if 0
	//对互斥锁进行初始化
	pthread_mutex_init(&a->mutex, NULL);
#else
	//对读写锁进行初始化
	//pthread_rwlock_init(&a->rwlock, NULL);
#endif

	sem_init(&a->sem, 0, 1);

	return a;
}

//销毁账户
extern void destroy_account(Account *a)
{
	assert(NULL != a);
#if 0
	//销毁互斥锁
	pthread_mutex_destroy(&a->mutex);
#else
	//销毁读写锁
	//pthread_rwlock_destroy(&a->rwlock);
#endif

	sem_destroy(&a->sem);

	free(a);
}

//取款
extern double withdraw(Account *a, double amt)
{
	assert(NULL != a);

	//对共享资源操作的临界区
	//pthread_mutex_lock(&a->mutex);	//对共享资源（账户）加锁

	//加写锁
	//pthread_rwlock_wrlock(&a->rwlock);

	//P(1)操作
	sem_wait(&a->sem);

	if (amt < 0 || amt > a->balance)
	{
		//释放互斥锁
		//pthread_mutex_unlock(&a->mutex);

		//释放写锁
		//pthread_rwlock_unlock(&a->rwlock);

		//V(1)操作
		sem_post(&a->sem);

		return 0.0;
	}
	double balance = a->balance;

	sleep(1);	//模拟取款过程

	balance -= amt;
	a->balance = balance;

	//释放互斥锁
	//pthread_mutex_unlock(&a->mutex);
	
	//释放写锁
	//pthread_rwlock_unlock(&a->rwlock);

	//V(1)操作
	sem_post(&a->sem);

	return amt;
}

//存款
extern double deposit(Account *a, double amt)
{
	assert(NULL != a);

	//pthread_mutex_lock(&a->mutex);
	//pthread_rwlock_wrlock(&a->rwlock);

	//P(1)操作
	sem_wait(&a->sem);

	if (amt < 0)
	{
		//pthread_mutex_unlock(&a->mutex);
		//pthread_rwlock_unlock(&a->rwlock);

		//V(1)操作
		sem_post(&a->sem);

		return 0.0;
	}

	double balance = a->balance;

	sleep(1);

	balance += amt;
	a->balance = balance;
	//pthread_mutex_unlock(&a->mutex);
	//pthread_rwlock_unlock(&a->rwlock);

	//V(1)操作
	sem_post(&a->sem);

	return amt;
}

//查看账户余额
extern double get_balance(Account *a)
{
	assert(NULL != a);

	//pthread_mutex_lock(&a->mutex);
	//pthread_rwlock_rdlock(&a->rwlock);
	
	//P(1)
	sem_wait(&a->sem);
	
	double balance = a->balance;
	//pthread_mutex_unlock(&a->mutex);
	//pthread_rwlock_unlock(&a->rwlock);
	
	//V(1)
	sem_post(&a->sem);

	return balance;
}


