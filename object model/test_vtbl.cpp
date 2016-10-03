#include <iostream>
#include <vector>
using namespace std;

class X{
public:
	virtual void f(){ cout << "X::f()" << endl;}
};

class A : public X{
public:
	virtual void f(){ cout << "A::f()" << endl;}
};

class B : public A{
public:
	void f(){ cout << "B::f()" << endl;}
};

class C : public B{
public:
	virtual void f(){ cout << "C::f()" << endl;}
};

int main(void)
{
	vector<void*> v;
	v.push_back(new X);
	v.push_back(new A);
	v.push_back(new B);
	v.push_back(new C);

	for(int i = 0; i < v.size(); ++i)
		static_cast<B*>(v[i])->f();
	cout << "hello cpp" << endl;
	return 0;
}