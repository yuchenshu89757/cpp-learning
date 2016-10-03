Declare non-member functions when type conversion should apply to all parameters

若所有参数皆需类型转换，请为此采用non-member函数

1.起因：在进行隐式转换时，只有当参数被列于参数列内，这个参数才是隐式类型转换的合格参与者。

假设你设计一个class用来表现有理数：

class Rational{
	
public:

	//构造函数刻意不为explicit, 允许int-to-Rational隐式转换
	Rational(int numerator = 0, int denominator = 0);
	
	//分子的访问函数
	int numerator()const;
	
	//分母的访问函数
	int denominator()const;
	
private:

	...
	
};

你想支持算术运算诸如加法、乘法等等，但你不确定是否该由member函数、non-member函数，或可能的话

由non-friend friend函数来实现它们。我们以 operator* 函数为例：

(1) member函数实现 operator*

class Rational{
	
public:

	...
	
	const Rational operator* (const Rational& rhs)const;
	
};

这个设计使你能够将两个有理数以最轻松自在的方式相乘：

Rational oneEighth(1, 8);

Rational oneHalf(1,2);

Rational result = oneHalf * oneEighth;	//OK

result = result * oneEighth;			//OK

然而当年尝试混合式运算，你会发现只有一半行得通：

result = oneHalf * 2;	//OK

result = 2 * oneHalf;	//Error

当以对应的函数形式重写上述两个式子，问题便一目了然：

result = oneHalf.operator*(2);	//OK

result = 2.operator*(oneHalf);	//Error

原因在于，oneHalf是一个内含operator*函数的class的对象，所以编译器调用该函数，然而2并没有相应

的class，也就没有operator*成员函数。编译器也会尝试寻找可被以下这般调用的non-member operator*：

result = operator* (2, oneHalf);

可惜的是，本例并不存在一个接受 int 和 Rational 作为参数的non-member operator*, 因此查找失败。

前面那两次调用，2一次被接受，另一次被拒绝，原因就在于发生了所谓隐式类型转换。编译器知道你正

在传递一个int, 而函数需要的是Rational; 但它发现Rational类有接受一个int的构造函数，于是它就调

用那个构造函数，将int变成了一个Rational对象，因而前面那次调用看起来像这样：

const Rational tmp(2);		//根据2建立一个临时性的Rational对象

result = oneHalf * tmp;		//等同于oneHalf.operator*(tmp)

当然，只因为涉及non-explicit构造函数，编译器才那么做，如果Rational构造函数是explicit，以下语

句没有一个可通过编译：

result = oneHalf * 2;	//Error，无法将2转换为一个Rational

result = 2 * oneHalf;	//Error

我们再来讨论什么情况下，隐式类型转换会起作用，对于第一个表达式：

result = oneHalf * 2;

之所以可以通过，是因为2恰好位于oneHalf的成员函数operator*(const Rational& rhs)的参数列内，即：

result = oneHalf.operator*(2);

此时，编译器通过隐式类型转换，发现2刚好可以转换成一个我们需要的Rational对象；

对于第二个表达式：

result = 2 * oneHalf;

2所在的地位相当于"被调用之成员函数所隶属的那个对象"--即this对象--的那个隐喻参数，编译器拒绝

执行隐式类型转换，因此第二个表达式无法通过编译。

(2)采用non-member函数实现 operator*:

class Rational{...};	//不包括 operator*

const Rational operator* (const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs.numerator() * rhs.numerator(), 
					lhs.denominator() * rhs.denominator());
}

Rational oneFourth(1,4);

Rational result;

result = oneFourth * 2;		//OK

result = 2 * oneFourth;		//OK

这种情况下，编译器就可以在每一个参数身上执行隐式类型转换。

(3)采用friend函数实现 operator*:

这种方式和non-member函数实现几乎完全一样，不同的是，friend函数可以访问Rational的private成员，

而non-member函数则只能访问public部分，从封装性角度来讲，推荐使用non-member函数的方式。
