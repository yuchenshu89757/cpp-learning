Item32 Make sure public inheritance models "is-a"

确定你的public继承塑模出is-a关系

1. 起因：以C++进行面向对象编程，"public继承"意味着is-a。适用于base class身上的每一件

事情一定也适用于derived class身上，因为每一个derived class对象也都是一个base class对

象。这也是设计模式中很重要的一个法则：里氏代换原则，所有父类出现的地方，子类一定可以

出现。

2. 总结：由于"public 继承"意味着is-a，因此我们在进行继承设计时，要格外注意那些游离于

真相边缘的假象，例如令正方形公开继承矩形，矩形的长宽可以独立变化，而正方形则不可以，

一旦你那样做了，就违背了is-a的原则，又如所有鸟都会飞，企鹅是鸟，但是企鹅不会飞，这里

让企鹅公开继承鸟类，也显然是不可行的。



Item33 Avoid hiding inherited namespace

避免遮掩继承而来的名称

1. 起因：C++名称遮掩规则--遮掩名称，主要是根据作用域来实施，和有无继承无关，和类型是

否相同无关，只和名称有关。

(1) 普通变量遮掩

int x;	//global变量

void someFunc()
{
	double x;		//local变量
	std::cin >> x;	//读一个新值赋予local变量x
}

这里名为x的double遮掩了一个名为x的int, 虽然两者类型不同。当编译器处理someFunc的作用域

内并遭遇名称x时，它在local作用域内查找是否有什么东西带着这个名称，如果找到就不再找其他

作用域。

(2)继承下的成员函数遮掩

class Base{
private:
	int x;
public:
	virtual void mf1() = 0;
	virtual void mf1(int);
	virtual void mf2();
	void mf3();
	void mf3(double);
	...
};

class Derived : public Base{
public:
	virtual void mf1();
	void mf3();
	void mf4();
	...
};

这里base class内所有名为mf1和mf3的函数都被derived class内的mf1和mf3函数遮掩掉了，从名

称查找观点来看，Base::mf1和Base::mf3不再被Derived继承。这里，虽然base classes和derived

classes内的函数有不同的参数类型也适用，而且不论函数是virtual或non-virtual一体适用。

现在假设derived class内的mf4的实现码部分像这样：
	
void Derived::mf4()
{
	...
	mf2();
	...
}

当编译器看到这里使用名称mf2，必须估算它指涉什么东西。编译器的做法是查找各作用域，看看有

没有某个名为Mf2的声明式。首先查找local作用域(mf4覆盖的作用域)，没有找到，接着查找其外围

作用域(class Derived覆盖的作用域)，也没有找到，于是再往外围移动，到了base class覆盖的作

用域。如果base class作用域内没有找到，查找动作便继续下去，首先找内含Base的那个namespace

的作用域，最后往global作用域找去。

2. 解决方案

(1) 使用using声明式，令继承而来的某给定名称之所有同名函数在derived class中都可见

class Derived : public Base{
public:
	using Base::mf1;	//让base class内名为mf1和mf3的所有东西
	using Base::mf3;	//在Derived作用域内都可见
	virtual void mf1();
	void mf3();
	void mf4();
	...
};

现在，继承机制将一如既往地运作：

Derived d;

int x;

d.mf1();	//调用Derived::mf1

d.mf1(x);	//调用Base::mf1

d.mf2();	//调用Base::mf2

d.mf3();	//调用Derived::mf3

d.mf3(x);	//调用Base::mf3

(2) 使用inline转交函数，将继承而得的名称汇入derived class作用域内

class Derived : private Base{
public:
	virtual void mf1()
	{
		Base::mf1();
	}
	...
};

Derived d;

int x;

d.mf1();	//正确，调用的是Derived::mf1

d.mf1(x);	//错误，Base::mf1()被遮掩了

3. 总结

(1) derived classes内的名称会遮掩base classes内的名称。在public继承下从来没有人希望如此。

(2) 为了让被遮掩的名称再见天日，可使用using声明式或转交函数。

