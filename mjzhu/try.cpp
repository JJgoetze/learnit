// try.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
using namespace std;


//计算100以下所有是3或者5的倍数的和
int SumBelow100()
{
	int sum=0;
	for(int i=0;i<1000;i++)
	{
		if(i%3==0||i%5==0)
			sum+=i;

	}
	return sum;
}

//计算斐波那契序列中小于400万的元素中所有是偶数的元素的和
int SumFibonacciBelow4000000()
{

	int sum=0;
	int a=1;
	int b=1;
	int c=a+b;
	int count=0;
	while(c<4000000)
	{
		sum+=c;
		a=b+c;
		b=c+a;
		c=a+b;
		count++;
	}
	cout<<count<<endl;
	return sum;
}
int _tmain(int argc, _TCHAR* argv[])
{
	cout<<SumBelow100()<<endl;
	cout<<SumFibonacciBelow4000000()<<endl;
	system("pause");
	return 0;
}

