Prefer pass-by-reference-to-const to pass-by-value_comp

宁以pass-by-reference-to-const替换pass-by-value

1.起因：缺省情况下C++以by value方式传递对象至函数。除非另外指定，否则函数参数都是以实际实参

的复件为初值，而调用端所获得的亦是函数返回值的一个复件。这些复件是由对象的copy构造函数产出，

这使得pass-by-value成为昂贵的操作。考虑以下class继承体系：

class Person{
	
public:
	
	Person();
	
	virtual ~Person();
	
	...
	
private:

	std::string name;
	
	std::string address;
	
};

class Student : public Person{
	
public:

	Student();
	
	~Student();
	
private:

	std::string schoolName;
	
	std::string schoolAddress;
	
};

现在考虑以下代码，其中调用函数validateStudent, 后者需要一个Student实参并返回它是否有效：

bool validateStudent(Student s);

Student sa;

bool saIsOk = validateStudent(sa);

当上述函数被调用时，Student的copy构造函数会被调用，以sa为蓝本将s初始化，由于Student继承

自Person，因此每次构造Student对象必须构造出一个Person对象，而Student对象内有两个string

对象，Person对象内也有两个string对象。当函数返回时，Student复件被销毁，又会调用相应的析

构函数。这样以pass-by-value方式传递一个Student对象会导致

如下的函数调用：

(1) 1次Person copy构造函数；

(2) 1次Student copy构造函数；

(3) 4次string copy构造函数；

(4) 4次string 析构函数；

(5) 1次Student 析构函数；

(6) 1次Person 析构函数；

因此，总成本是"6次构造函数和6次析构函数"，为了回避那些构造和析构动作，我们采用

pass-by-reference-to-const的方式：

bool validateStudent(const Student& s);

这种传递方式效率高很多，而且没有任何构造和析构函数被调用，没有任何新对象被创建。

pass-by-reference-to-const有两个有点：

(1) const 确保函数内不会对传入的Student对象作任何改变，符合pass-by-value调用的初衷；

(2) by-reference方式可以避免对象切割问题，这在多态情况下尤其突出。当一个derived class

对象以by-value方式传递并被视为一个base class对象，base class的copy构造函数会被调用，而

"造成此对象的行为像个derived class对象"的那些特化性质全被切割掉了，仅仅留下一个base class

对象。假设你在一组classes上工作，用来实现一个图形窗口系统：

class Window{
	
public:

	...
	
	std::string name()const;
	
	virtual void display()const;
	
};

class WindowWithScrollBars : public Window{
	
public:

	...
	
	virtual void display()const;
	
};

假设现在写个函数打印窗口名称，然后显示该窗口。

void printNameAndDisplay(Window w)
{
	
	std::cout << w.name;
	
	w.display();
	
}

当你按照下述方式调用时，

WindowWithScrollBars wwsb;

printNameAndDisplay(wwsb);

函数内将永远是一个Window对象，而非WindowWithScrollBars，wwsb的特化信息会被切除掉，剩下

的Window对象的部分会传入函数内，因此调用的display永远是Window::display。解决切割的办法

是，采用pass-by-reference-to-const的方式传递w:

void printNameAndDisplay(const Window& w)
{
	
	std::cout << w.name;
	
	w.display();
	
}

现在，传进来的窗口是什么类型，w就表现出那种类型。

*在C++编译器底层，references往往以指针实现，因此pass-by-reference通常意味真正传递的是

指针。因此如果有个对象属于内置类型，pass-by-value往往比pass-by-reference的效率高些。

对于STL的迭代器和函数对象，习惯上也被设计为pass-by-value。除此之外的类型，尽量以pass

by reference to const替换pass by value。
