#include <iostream>
using namespace std;

typedef void (*Fun)(void);

class Parent{
public:
    int i_parent;
    Parent():i_parent(10){}
    virtual void f(){ cout << "Parent::f()" << endl;}
    virtual void g(){ cout << "Parent::g()" << endl;}
    virtual void h(){ cout << "Parent::h()" << endl;}
};

class Child : public Parent{
public:
    int i_child;
    Child():i_child(100){}
    virtual void f(){ cout << "Child::f()" << endl;}
    virtual void g_child(){ cout << "Child::g_child()" << endl;}
    virtual void h_child(){ cout << "Child::h_child()" << endl;}
};

class GrandChild : public Child{
public:
    int i_grandchild;
    GrandChild():i_grandchild(1000){}
    virtual void f(){ cout << "GrandChild::f()" << endl;}
    virtual void g_child(){ cout << "GrandChild::g_child()" << endl;}
    virtual void h_grandchild(){ cout << "GrandChild::h_grandchild()" << endl;}
};


int main(void)
{
    Fun pFun;
    GrandChild gc;
    int **pVtbl = (int**)&gc;

    cout << "[0] GrandChild::_vptr->" << endl;
    for(int i = 0; (pFun = (Fun)pVtbl[0][i]) != NULL; ++i)
    {
        cout << "   [" << i << "] ";
        pFun();
    }
    cout << "[1] Parent.i_parent = " << (int)pVtbl[1] << endl;
    cout << "[2] Child.i_child = " << (int)pVtbl[2] << endl;
    cout << "[3] GrandChild.i_grandchild = " << (int)pVtbl[3] << endl;
    return 0;
}