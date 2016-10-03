Understand the ins and outs of inlining

透彻了解inlining的里里外外

1. inline函数

(1) 将"对此函数的每一个调用"都以函数本体替换之;

(2) 免除函数调用成本;

(3) 编译器可以对它执行语境相关最优化；

(4) 过度使用易导致代码膨胀；

(5) inline只是对编译器的一个申请，不是强制命令。

(6) inline函数无法随着程序库的升级而升级

2. 如何声明inline函数

(1) 定义于class内的成员函数和friend函数，被隐喻声明为inline;

(2) 通过在函数定义式前加关键字inline来明确声明inline函数；

template <typename T>		//明确申请inline
inline const T& std::max(const T& a, const T& b)
{
	return a < b ? b : a;
}

*inline函数和templates两者通常都被定义于头文件内

3. 编译器行为: 一个表面上看似inline的函数是否真是inline, 取决于编译器

(1) 如果程序要取某个inline函数的地址，编译器通常必须为此函数生成一个outlined函数本体。

与此并提的是，编译器通常不对"通过函数指针而进行的调用"实施inlining，这意味着inline函数

的调用有可能被inlined, 也可能不被inlined, 取决于该调用的实施方式：

inline void f(){...}	//假设编译器有意愿inline对f的调用

void (*pf)() = f;		//pf指向f

...

f();					//这个调用被inlined, 因为它是一个正常调用

pf();					//这个调用或许不被inlined, 因为它是通过函数指针达成

(2) 有时候编译器会生成构造函数和析构函数的outline副本，如此一来它们就可以获得指针指向

那些函数，在array内部元素的构造和析构过程中调用。

4. 总结

将大多数inlining限制在小型、被频繁调用的函数身上。这可使日后的调试过程和二进制升级更容

易，也可使潜在的代码膨胀问题最小化，使程序的速度提升机会最大化。
