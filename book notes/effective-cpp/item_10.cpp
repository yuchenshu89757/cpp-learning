Have assignment operators return a reference to *this

令 operator= 返回一个reference to *this

1. 起因：为了实现"连锁赋值"，赋值操作符必须返回一个reference指向操作符

的左侧实参，这是为classes实现赋值操作符时应该遵循的协议。

2. 举例：关于赋值，我们可以把它们写成连锁形式：

int x, y, z;

x = y = z = 5;	//赋值连锁形式

由于赋值采用由结合律，所以上述连锁赋值被解析为：

x = (y = (z = 5));

***这个协议不仅适用于标准赋值形式，也适用于所有赋值相关运算。例如：

class Widget{
	
public:

	...
	
	Widget& operator=(const Widget& rhs)	//标准赋值形式
	{
		...
		return *this;
	}
	
	Widget& operator=(int rhs)				//非标准赋值形式
	{
		...
		return *this;
	}
	
	Widget& operator+=(const Widget& rhs)	//复合赋值运算符
	{
		...
		return *this;
	}
	
};

Tips: 这只是个协议，并无强制性，但为了与所有内置类型和标准程序库兼容，

最好还是遵循它。
