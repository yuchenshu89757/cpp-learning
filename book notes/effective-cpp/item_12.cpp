Copy all parts of an object

复制对象时勿忘其每一个成分

1. 起因：对于自定义的copyging函数(copy constructor和copy assignment operator),

容易遗漏某些成员的拷贝而造成局部拷贝，编译器对局部拷贝不发出任何警告，而在继承

体系中，局部拷贝更容易发生。

2. 举例：(1)考虑一个class用来表现客户，其中手写copying函数

class Customer{
	
public:

	...
	
	Customer(const Customer&);
	
	Customer& operator=(const Customer&);
	...
	
private:

	std::string name;
	
};

Customer::Customer(const Customer& rhs):name(rhs.name){}

Customer& Customer::operator=(const Customer& rhs)
{
	name = rhs.name;
	return *this;
}

如果后来发现遗漏了顾客的出生日期，于是

class Date{...};

class Customer{
	
public:

	...
	
private:

	std::string name;
	
	Date birthDay;
	
};

这个时候，需要修改copying函数，以实现对新加入成员birthDay的拷贝，如果没有这么

做，编译器也不会提醒你。

(2)如果发生继承，事情可能变得更糟：

class VipCustomer : public Customer{
	
public:

	...
	
	VipCustomer(const VipCustomer&);
	
	VipCustomer& operator=(const VipCustomer&);
	
	...
	
private:

	int priority;
	
};

此时，正确的做法是调用 base classes 内的适当的copying函数

VipCustomer::VipCustomer(const VipCustomer& rhs)
: Customer(rhs),				//调用base class的copy构造函数
  priority(rhs.priority)
  {}
  
VipCustomer& VipCustomer::operator=(const VipCustomer& rhs)
{
	Customer::operator=(rhs);	//对base成分进行赋值动作
	priority = rhs.priority;
	return *this;
}

*总之一点，当编写一个copying函数，请确保：I.复制所有local成员变量；

II.调用所有base classes内适当的copying函数

(3)不要尝试以某个copying函数实现另一个copying函数。应该将共同机能放进第

三个函数中，并由两个copying函数共同调用。

