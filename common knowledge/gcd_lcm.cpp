#include <iostream>
using namespace std;

int gcd(int a, int b)
{
	if(a % b == 0)return b;
	return gcd(b, a % b);
}

//the following method can advoid the division between big integers.
int kgcd(int a, int b)
{
	if(0 == b)return a;
	if(0 == a)return b;
	if(!(a & 1) && !(b & 1))return kgcd(a >> 1, b >> 1) << 1;
	else if(!(a & 1))return kgcd(a >> 1, b);
	else if(!(b & 1))return kgcd(a, b >> 1);
	return kgcd(abs(a - b), min(a, b));
}
//lowest common multiple
int lcm(int a, int b)
{
	return a / kgcd(a, b) * b;
}

int main(void)
{
	int a, b;
	while(cin >> a >> b){
		cout << "gcd = " << gcd(a, b) << endl;
		cout << "kgcd = " << kgcd(a, b) << endl;
		cout << "lcm = " << lcm(a, b) << endl;
	}
	return 0;
}