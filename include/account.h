#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <pthread.h>
#include <semaphore.h>

typedef struct
{
	int		code;
	double	balance;
#if 0
	//定义一把互斥锁，用来对多线程操作的银行账户（共享资源）进行加锁（保护）的
	pthread_mutex_t mutex;

	/*
	 *	建议互斥锁用来锁定一个账户(共享资源)，
	 *	和账户绑定在一起，
	 *	尽量不设置成全局变量，否则可能会出现一把锁去锁多个账户
	 *	这样会导致程序的并发性降低
	 */
#else
	//定义一把读写锁
	//pthread_rwlock_t rwlock;
#endif
	sem_t	sem;

}Account;

//创建账户
extern Account * create_account(int code, double balance);

//销毁账户
extern void destroy_account(Account *a);

//取款
extern double withdraw(Account *a, double amt);

//存款
extern double deposit(Account *a, double amt);

//查看账户余额
extern double get_balance(Account *a);

#endif

