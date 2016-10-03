Know what functions C++ silently writes and calls

了解C++默认编写并调用哪些函数

1. 一个empty class, C++编译器会自动为其声明4个函数

(1) public inline default constructor;

(2) public inline copy constructor;

(3) public inline copy assignment operator;

(4) public inline destructor;

例如，如果您写下：

class Empty{};

C++编译器会帮你扩展成：

class Empty{
	
public：

	Empty();
	
	Empty(const Empty&);
	
	Empty& operator=(const Empty&);
	
	~Empty();
	
}；

Tips：这里编译器仅仅只是帮你声明这些函数，并未给出函数的定义，惟有当这些函数

被调用，它们才会被编译器创建（定义）出来。

Empty e1;		//default constructor被定义并调用

Empty e2(e1);	//copy constructor被定义并调用

e2 = e1;		//copy assignment operator被定义并调用

对于上述4个函数中的一个或多个，如果用户自己提供了定义，编译器自动定义的行为将被压制，

而对于用户没有定义的函数，编译器是否自动帮你生成，要视情况而定：

(1)对于内含reference成员或const成员的类，编译器拒绝生成拷贝构造函数和拷贝赋值运算符

(2)如果某个base class将copy assignment操作符声明为private，编译器将拒绝为其derived

classes生成一个copy assignment操作符

