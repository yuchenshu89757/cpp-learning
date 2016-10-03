#include <iostream>
using namespace std;

typedef void (*Fun)(void);

class Base1{
public:
    int ibase1;
    Base1():ibase1(10){}
    virtual void f(){ cout << "Base1::f()" << endl;}
    virtual void g(){ cout << "Base1::g()" << endl;}
    virtual void h(){ cout << "Base1::h()" << endl;}
};
class Base2{
public:
    int ibase2;
    Base2():ibase2(20){}
    virtual void f(){ cout << "Base2::f()" << endl;}
    virtual void g(){ cout << "Base2::g()" << endl;}
    virtual void h(){ cout << "Base2::h()" << endl;}
};
class Base3{
public:
    int ibase3;
    Base3():ibase3(30){}
    virtual void f(){ cout << "Base3::f()" << endl;}
    virtual void g(){ cout << "Base3::g()" << endl;}
    virtual void h(){ cout << "Base3::h()" << endl;}
};

class Derived : public Base1, public Base2, public Base3{
public:
    int iderived;
    Derived():iderived(100){}
    virtual void f(){ cout << "Derived::f()" << endl;}
    virtual void g1(){ cout << "Derived::g1()" << endl;}
};


int main(void)
{
    int offset;
    Fun pFun;
    Derived d;
    int **pVtbl = (int**)&d;

    cout << "[0] Base1::_vptr->" << endl;
    for(int i = 0; i < 4; ++i)
    {
        pFun = (Fun)pVtbl[0][i];
        cout << "\t[" << i << "] ";
        pFun();
    }
    pFun = pFun = (Fun)pVtbl[0][4];
    cout << "\t[4] " << pFun << endl;
    cout << "[1] Base1.ibase1 = " << (int)pVtbl[1] << endl;

    offset = sizeof(Base1) / 4;

    cout << "[" << offset << "] Base2::_vptr->" << endl;
    for(int i = 0; i < 3; ++i)
    {
        pFun = (Fun)pVtbl[offset][i];
        cout << "\t[" << i << "] ";
        pFun();
    }
    pFun = pFun = (Fun)pVtbl[offset][3];
    cout << "\t[3] " << pFun << endl;
    cout << "[" << offset+1 << "] Base2.ibase2 = " << (int)pVtbl[ offset+1] << endl;

    offset += sizeof(Base2) / 4;

    cout << "[" << offset << "] Base3::_vptr->" << endl;
    for(int i = 0; i < 3; ++i)
    {
        pFun = (Fun)pVtbl[offset][i];
        cout << "\t[" << i << "] ";
        pFun();
    }
    pFun = pFun = (Fun)pVtbl[offset][3];
    cout << "\t[3] " << pFun << endl;
    cout << "[" << offset+1 << "] Base3.ibase3 = " << (int)pVtbl[ offset+1] << endl;
    cout << "[" << offset+2 << "] Derived.iderived = " << (int)pVtbl[ offset+2] << endl;

    return 0;
}