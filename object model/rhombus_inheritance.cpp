#include <iostream>
using namespace std;

typedef void (*Fun)(void);

class Base{
public:
    int ib;
    char cb;
    Base():ib(123), cb('B'){}
    virtual void f(){ cout << "Base::f()" << endl;}
    virtual void Bf(){ cout << "Base::Bf()" << endl;}
};
class Base1 : virtual public Base{
public:
    int ib1;
    char cb1;
    Base1():ib1(1234), cb1('1'){}
    virtual void f(){ cout << "Base1::f()" << endl;}
    virtual void f1(){ cout << "Base1::f1()" << endl;}
    virtual void Bf1(){ cout << "Base1::Bf1()" << endl;}
};
class Base2{
public:
    int ib2;
    char cb2;
    Base2():ib2(1235), cb2('2'){}
    virtual void f(){ cout << "Base2::f()" << endl;}
    virtual void f2(){ cout << "Base2::f2()" << endl;}
    virtual void Bf2(){ cout << "Base2::Bf2()" << endl;}
};

class Derived : public Base1, public Base2{
public:
    int id;
    char cd;
    Derived():id(100), cd('D'){}
    virtual void f(){ cout << "Derived::f()" << endl;}
    virtual void f1(){ cout << "Derived::f1()" << endl;}
    virtual void f2(){ cout << "Derived::f2()" << endl;}
    virtual void Df(){ cout << "Derived::Df()" << endl;}
};


int main(void)
{
    int offset;
    Fun pFun;
    Derived d;
    Base1 b1;
    int **pVtbl;

    //single virtual inheritance
    cout << "single virtual inheritance:" << endl;
    pVtbl = (int**)&b1;
    cout << "[0] Base1::_vptr->" << endl;
    for(int i = 0; (pFun = (Fun)pVtbl[0][i]) != NULL; ++i)
    {
        cout << "\t[" << i << "] ";
        pFun();
    }
    pFun = (Fun)pVtbl[0][3];
    cout << "\t[3] " << pFun << endl;
    cout << "[1] Base1.ib1 = " << (int)pVtbl[1] << endl; 
    cout << "[2] Base1.cb1 = " << (char)(int)pVtbl[2] << endl; 

    cout << "[3] Base::_vptr->" << endl;
    for(int i = 0; (pFun = (Fun)pVtbl[3][i]) != NULL; ++i)
    {
        cout << "\t[" << i << "] ";
        pFun();
    }
    pFun = (Fun)pVtbl[3][2];
    cout << "\t[3] " << pFun << endl;
    cout << "[4] Base.ib = " << (int)pVtbl[4] << endl; 
    cout << "[5] Base.cb = " << (char)(int)pVtbl[5] << endl; 

    return 0;
}