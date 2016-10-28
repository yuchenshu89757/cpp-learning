/*
题目：n个灯排成一排，第1轮将所有灯泡打开，第2轮将下标是2的倍数的灯状态反转，第3轮将下标是3的倍数的灯状态反转；
	  依次类推，第100轮结束的时候，还有几盏灯亮着。
解法：实际求n以内的完全平方数的个数
*/
#include <iostream>
#include <memory.h>
using namespace std;

const int N = 10000;
bool bulb[N];

void printBulb(int n)
{
	for(int i = 1; i <= n; ++i)
		if(bulb[i])cout << i << " ";
	cout << endl;
}

void openClose(int n)
{
	for(int i = 1; i <= n; ++i){
		for(int j = 1; j <= n; ++j){
			if(j % i == 0)
				bulb[j] = !bulb[j];
		}
	}
}

int main(void)
{
	int n;
	while(cin >> n){
		memset(bulb, 0, sizeof(bulb));
		openClose(n);
		printBulb(n);
	}
    return 0;
}