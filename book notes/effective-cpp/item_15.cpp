Provide access to raw resources in resource-managing classes

在资源管理类中提供对原始资源的访问

1.起因：资源管理类虽然排除了资源泄漏的隐患，但有的时候应用程序需要绕过这层壁垒直接访问

原始资源，因此每一个RAII class 应该提供一个"取得其所管理之资源"的办法。

2. 关于之前介绍的两个RAII类，auto_ptr和tr1::shared_ptr

(1) 都提供一个get成员函数，用来执行显式转换，也就是它会返回智能指针内部的原始附件(的复件)；

例如，对于下面这个函数：

int daysHeld(const Investment* pi);

如果我们有

std::auto_ptr<Investment> pInv(createInvestment());

或者std::tr1::shared_ptr<Investment> pInv(createInvestment());

你想这么调用它：

int days = daysHeld(pInv);

却通不过编译，因为daysHeld需要的是Investment*指针，正确的方法是：

int days = daysHeld(pInv.get());

(2)都重载了指针取值操作符(operator* 和 operator->)，它们允许隐式转换至底部原始指针：

class Investment{
	
public:

	bool isTaxFree()const;
	
	...

};

Investment* createInvestment();

std::tr1::shared_ptr<Investment> pi1(createInvestment());

bool taxable1 = !(pi1->isTaxFree());

std::auto_ptr<Investment> pi2(createInvestment());

bool taxable2 = !((*pi2).isTaxFree());

3. 总结：对原始资源的访问可能经由显式转换或隐式转换。一般而言显式转换比较安全，但隐式

转换对客户比较方便。
