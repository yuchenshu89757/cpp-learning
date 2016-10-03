Make interfaces easy to use correctly and hard to use incorrectly

让接口容易被正确使用，不易被误用

1.导入新类型

假设为一个用来表现日期的class设计构造函数：

class Date{
	
public:

	Date(int month, int day, int year);
	
	...
	
};

这个设计存在两个很明显的隐患：

(1) 用户也许会以错误的次序传递参数；

Date d(30, 3, 1995);

(2) 用户可能传递一个无效的月份或天数；

Date d(2, 30, 1995);

可以通过导入简单的外覆类型来区别天数、月份和年份，然后于Date构造函数中使用这些类型：

struct Day{
	
	int val;
	
	explicit Day(int d): val(d){}
	
};

struct Month{
	
	int val;
	
	explicit Month(int m): val(m){}
	
};

struct Year{
	
	int val;
	
	explicit Year(int y): val(y){}
	
};

class Date{
	
public:

	Date(const Month&, const Day&, const Year&);
	
	...
	
};

于是当遇到下述情况时，会造成编译不通过

Date d1(30, 3, 1995);		//错误，不正确的类型

Date d2(Day(30), Month(3), Year(1995));	//错误，不正确的类型

Date d3(Month(3), Day(30), Year(1995));	//正确

I. 一旦正确的类型就位，限制其值有时候是通情达理的，例如一年只有12个有效月份，所以Month

应该反映这一事实，办法之一是利用 enum 表现月份，但 enum 不具备我们希望拥有的安全性，

比较安全的做法是预先定义所有有效的Months：

class Month{
	
public:

	static Month Jan(){return Month(1);}	//以函数替换对象，表现某个特定月份
	
	static Month Feb(){return Month(2);}
	
	...
	
	static Month Dec(){return Month(12);}
	
private:

	explicit Month(int m);		//阻止生成新的月份

	...
	
};

Date d(Month::Mar(), Day(1), Year(1995));

2. 限制类型内什么事可做，什么事不能做。尽量令你的类types行为与内置types一致。

3. 消除客户的资源管理责任。
