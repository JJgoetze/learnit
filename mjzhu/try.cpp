// try.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
using namespace std;


//����100����������3����5�ı����ĺ�
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

//����쳲�����������С��400���Ԫ����������ż����Ԫ�صĺ�
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

//�ж��Ƿ�������
bool IsPrime(long long num)
{
	if(num<0)
		return false;
	if(num==1||num==2)
		return true;
	for(long long i=2;i*i<=num;i++)
		if(num%i==0)
			return false;
	return true;

}



//�ҳ�һ���������������
int FindLargestPrimeFactor(long long num)
{
	long long i=2;
	long long pre_i=2;
	int count=0;
	for(;i<num;i++)
	{
		if(num%i==0)
		{
			if(IsPrime(i))
			{
				if(i>pre_i)
					pre_i=i;
				
			}
			num=num/i;
			i=2;
			
		}
		count++;

	}
	cout<<count<<endl;
	return num;


}
int _tmain(int argc, _TCHAR* argv[])
{
	cout<<SumBelow100()<<endl;
	cout<<SumFibonacciBelow4000000()<<endl;
	cout<<FindLargestPrimeFactor(600851475143)<<endl;
	system("pause");
	return 0;
}

