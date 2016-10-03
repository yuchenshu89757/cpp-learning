Handle assignment to self in operator=

在 operator= 中处理自我赋值

1. 起因：将对象赋值给自己时发生自我赋值，自我赋值容易引发安全问题。

假设创建一个class用来保存一个指针指向一块动态分配的位图：

class Bitmap{...};

class Widget{
	
	...
	
private:

 Bitmap *pb;	//指向一个从heap分配而得的对象
 
};

以下是针对Widget对象各种版本的赋值运算符：

(1)版本一，既不具备自我赋值安全性，也不具备异常安全性

Widget& operator=(const Widget& rhs)
{
	delete pb;
	pb = new Bitmap(*rhs.pb);
	return *this;
}

*如果 operator= 函数内的 *this 和 rhs指向同一个对象，delete语句不仅销毁

了当前对象的bitmap，也销毁了rhs的bitmap。在函数末尾，Widget原本不该被自

我赋值动作改变的，发现自己持有一个指针指向一个已被删除的对象。

(2)版本二，引入"证同测试"，消除了自我赋值的隐患，但不具备异常安全性

Widget& operator=(const Widget& rhs)
{
	if(this == &rhs)return *this;
	delete pb;
	pb = new Bitmap(*rhs.pb);
	return *this;
}

*如果"new Bitmap"导致异常，Widget最终会持有一个指针指向一块被删除的Bitmap。

(3)版本三，让 operator= 获得异常安全性，自我赋值安全性自动获得

Widget& operator=(const Widget& rhs)
{
	Bitmap *pOrg = pb;			//记住原来的pb
	pb = new Bitmap(*rhs.pb);	//令pb指向*pb的一个复件
	delete pOrg;				//删除原来的pb
	return *this;
}

*如果"new Bitmap"抛出异常,pb保持原状，即使没有证同测试，这段代码还是能够处理

自我赋值，虽然不是最高效的方法，但至少行得通。

(4)版本四，使用copy and swap技术

Widget& operator=(const Widget& rhs)
{
	Widget tmp(rhs);	//为rhs数据制作一份复件
	swap(tmp);			//将*this数据和上述复件的数据交换
	return *this;
}

*这种方法不仅异常安全，而且自我赋值安全。
