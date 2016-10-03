#include <iostream>
using namespace std;

typedef void (*Fun)(void);
const int FUN_NUM = 2;

class Base{
public:
    virtual void f(){ cout << "Base::f()" << endl;}
    virtual void g(){ cout << "Base::g()" << endl;}
};

int main(void)
{
    Base b;

    printf("The address of virtual table: 0x%p\n", (int*)&b); 
    printf("The address of first function in virtual table: 0x%p\n", (int*)*(int*)(&b)); 

    //case 1:
    Fun pFun = (Fun)*((int*)*(int*)(&b));
    pFun();
    //case 2:
    Fun pFun_1 = (Fun)((int*)*(int*)(&b))[0];
    pFun_1();
    //case 3:
    int *pVtbl = (int*)*(int*)(&b);
    for(int i = 0; i < FUN_NUM; ++i)
    {
        Fun pmFun = (Fun)pVtbl[i];
        pmFun();
    }
    //case 4:
    int **pVtable = (int**)&b;
    for(int i = 0; i < FUN_NUM; ++i)
    {
        Fun pmFun = (Fun)pVtable[0][i];
        pmFun();
    }
    return 0;
}