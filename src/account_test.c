#include "account.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int sleep();

typedef struct
{
	char	name[20];
	Account *account;
	double	amt;
}OperArg;

//定义取款操作的线程运行函数
void * withdraw_fn(void *arg)
{
	OperArg *oa = (OperArg *)arg;
	double amt = withdraw(oa->account, oa->amt);

	printf ("%8s(0x%lx) withdraw %f from account %d\n", oa->name, pthread_self(), amt, oa->account->code);

	return (void *)0;
}

//定义存款操作的线程运行函数
void * deposit_fn(void * arg)
{
	
	OperArg *oa = (OperArg *)arg;
	double amt = deposit(oa->account, oa->amt);

	printf ("%8s(0x%lx) deposit %f from account %d\n", oa->name, pthread_self(), amt, oa->account->code);

	return (void *)0;
}

//定义检查银行账户的线程运行函数
void * check_fn(void *arg)
{
	
	return (void *)0;	
}

int main(void)
{
	int err;
	pthread_t boy, girl;
	Account *a = create_account(100001, 10000);

	OperArg o1, o2;

	strcpy(o1.name, "boy");
	o1.account = a;
	o1.amt = 10000;

	strcpy(o2.name, "girl");
	o2.account = a;
	o2.amt = 10000;

	//启动两个线程（boy和girl线程）
	//同时操作同一个银行账户
	
	err = pthread_create(&boy, NULL, withdraw_fn, (void *)&o1);
	if (err != 0)
	{
		perror("pthread_create error!");
	}

	err = pthread_create(&girl, NULL, withdraw_fn, (void *)&o2);
	if (err != 0)
	{
		perror("pthread_create error!");
	}

	pthread_join(boy, NULL);
	pthread_join(girl, NULL);

	printf ("account balance: %f\n", get_balance(a));
	destroy_account(a);

	return 0;
}


