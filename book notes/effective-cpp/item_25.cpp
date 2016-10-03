Consider support for a non-throwing swap

考虑写出一个不抛出异常的swap函数

1.起因：swap是一个非常有用的函数，原本它只是STL的一部分，而后成为异常安全性编程的脊柱，以及

用来处理自我赋值的一个常见机制。由于swap如此有用，适当的实现很重要，然而在非凡的重要性之外它

也带来了非凡的复杂度。

(1)缺省情况下，swap动作可由标准程序库提供swap算法完成：

namespace std{
	template <typename T>
	void swap(T& a, T& b)
	{
		T tmp(a);
		a = b;
		b = tmp;
	}
}

只要类型T支持copying, 缺省的swap实现代码就会帮你置换类型为T的对象，你不需要为此另外再做工作。

但对某些类型而言，这些复制动作无一必要；对他们而要swap缺省行为等于把高速铁路铺在慢速小巷内。

(2)对于"以指针指向一个对象，内含真正数据"的那种类型，通常的做法是"pimpl手法"(pointer to

implementation)。如果以这种手法设计Widget class, 看起来会像这样：

class WidgetImpl{
	
public:

	...
	
private:

	int a, b, c;			//可能有许多数据
	
	std::vector<double> v;	//意味复制时间很长
	
	...
};

class Widget{
	
public:

	Widget(const Widget& rhs);
	
	Widget& operator=(const Widget& rhs)
	{
		...
		*pImpl = *(rhs.pImpl);
		...
	}
	
	...
	
private:

	WidgetImpl* pImpl;		//指针，所指对象内含Widget数据
	
};

一旦要置换两个Widget对象值，我们唯一需要做的就是置换其pImpl指针，但缺省的swap算法不知道这一点，

因此我们希望告诉std::swap, 当Widgets被置换时真正该做的是置换其内部的pImpl指针。确切实践这个思路

的一个做法是：将std::swap针对Widget特化。下面是基本构想，但目前这个形式无法通过编译：

namespace std{
	//这是std::swap针对"T是Widget"的特化版本
	template<>
	void swap<Widget>(Widget& a, Widget& b)
	{
		swap(a.pImpl, b.pImpl);	//置换Widgets时只要置换它们的pImpl指针就好
	}
}

这个函数一开始的"template<>"表示它是std::swap的一个全特化版本，函数名称之后的"<Widget>"表示这一

特化版本系针对"T是Widget"而设计。通常我们不能改变std命令空间内的任何东西，但可以为标准templates

制造特化版本，使它专属于我们自己的classes。但这个函数无法通过编译，因为它企图访问a和b内的pImpl

指针，而那却是private。我们可以将这个特化版本声明为friend，但和以往的规矩不太一样：我们令Widget

声明一个名为swap的public成员函数做真正的置换工作，然后将std::swap特化，令它调用该成员函数：

class Widget{
	
public:

	...
	
	void swap(Widget& other)
	{
		using std::swap;
		swap(pImpl, other.pImpl);
	}
	
	...
	
};

namespace std{
	//修订后的std::swap特化版本
	template <>
	void swap<Widget>(Widget& a, Widget& b)
	{
		//若要置换Widgets，调用其swap成员函数
		a.swap(b);
	}
}

这种做法不仅能通过编译，还与STL容器有一致性，因为所有STL容器也都提供public swap成员函数和

std::swap特化版本（用以调用前者）。

然而假设Widget和WidgetImpl都是class templates而非classes，我们可以将WidgetImpl内的数据类型

加以参数化：

template <typename T>
class WidgetImpl{...};

template <typename T>
class Widget{...};

此时在Widget内放个swap成员函数像以往一样简单，但我们却在特化std::swap时遇上麻烦。我们如果写

成这样：

namespace std{
	template <typename T>
	void swap<Widget<T>>(Widget<T>& a, Widget<T>& b)
	{
		a.swap(b);
	}
}

看起来合情合理，却不合法。原因在于，C++只允许对class templates偏特化，在function templates身

上偏特化是不被允许的。当你打算偏特化一个function template时，惯常做法是简单地为它添加一个重

载版本，像这样：

namespace std{
	template <typename T>
	void swap(Widget<T>& a, Widget<T>& b)
	{
		a.swap(b);
	}
}

但很遗憾的是，这也是不合法的，由于std是个特殊的命名空间，客户可以全特化std内地额templates，

但不可以添加新的templates到std里头。

为了解决这个问题，我们不再将那个non-member swap声明为std::swap的特化版本或重载版本。为求简化，

假设Widget所有相关机能都被置于命名空间WidgetStuff内，整个结果看起来便像这样：

namespace WidgetStuff{
	...
	template <typename T>
	class Widget{...};	//同前，内含swap成员函数
	...
	template <typename T>
	void swap(Widget<T>& a, Widget<T>& b)
	{
		a.swap(b);
	}
}

(3)swap的调用，假设你正在写一个function template， 其内需要置换两个对象值，我们希望的是优先

调用T专属版本，并在该版本不存在的情况下调用std内的一般化版本：

template <typename T>
void doSomething(T& obj1, T& obj2)
{
	using std::swap;	//令std::swap在此函数内可用
	...
	swap(obj1, obj2);	//为T型对象调用最佳swap版本
}

准则是：调用时，首先针对std::swap使用using声明式，然后调用swap并不带任何"命名空间资格修饰"。
