Item06 : Explicitly disallow the use of compiler-generated functions you do not want

若不想使用编译器自动生成的函数，就该明确拒绝

1. 将成员函数声明为private而且故意不实现他们

class HomeForSale{
	
public:

	...
	
private:

	...
	
	HomeForSale(const HomeForSale&);			//只有声明
	
	HomeForSale& operator=(const HomeForSale&);	//只有声明
	
};

有了上述class定义，当用户企图拷贝HomeForSale对象，编译器会阻挠他。如果你不慎在

member函数或friend函数之内那么做，连接器会抛出错误。

2. 专门设计一个阻止copying动作的base class，将其copy constructor和copy assignment

操作符声明为private并且不予实现，任何想要阻止copying行为的类直接继承该类即可。

class Uncopyable{
	
protected:

	Uncopyable(){}		//允许derived对象构造和析构
	
	~Uncopyable(){}
	
private:

	Uncopyable(const Uncopyable&);	//但阻止copying
	
	Uncopyable& operator=(const Uncopyable&);
	
};

为求阻止HomeForSale对象被拷贝，我们唯一需要做的就是继承Uncopyable:

//class不再声明copy构造函数和copy assignment操作符

class HomeForSale : private Uncopyable{...};
