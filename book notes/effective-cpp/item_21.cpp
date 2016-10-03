Do not try to return a reference when you must return an object

必须返回对象时，别妄想返回其reference

1.起因：虽然pass-by-value代价昂贵，但也并不是一无是处；同样，pass-by-reference效率很高，

但并不是处处适用，有些时候，pass-by-reference可能会造成致命的错误。

2.举例，考虑一个用以表现有理数的class, 内含一个函数用来计算两个有理数的乘积

class Rational{
	
public:

	Rational(int numerator = 0, int denominator);
	
	...
	
private:

	int n, d;
	
	friend const Rational operator* (const Rational& lhs, const Rational& rhs);
	
};

这个版本的 operator* 系以by-value方式返回其计算结果，但往往有人觉得这样做还不够高效，

于是又萌生出以下几种看似高效，实则大错特错的实现方式。

(1)通过引用返回指向函数内local对象(on-the-stack)

const Ratinal& operator* (const Rational& lhs, const Rational& rhs)
{
	Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
	return result;
}

这个函数返回一个reference指向result, 但result是个local对象，在函数退出前就被销毁了，

于是它返回的reference指向一个被销毁的对象，其结果是无定义的行为。

(2)通过引用返回指向函数内动态分配的对象(on-the-heap)

const Rational& operator* (const Rational& lhs, const Rational& rhs)
{
	Rational* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
	return *result;
}

这个函数在heap内构造一个对象，然后通过引用返回它，这样就避免了函数退出前被销毁，但这

又带来了新的问题，谁该对着被你new出来的对象实施delete? 即使调用者诚实谨慎，并且出于良

好意识，他们还是不太能够在这样合情合理的用法下阻止内存泄漏：

Rational w, x, y, z;

w = x * y * z;

这里，同一个语句内调用了两次operator*, 也就需要两次delete，但却没有合理的办法让operator*

使用者进行那些delete调用， 因为没有合理你的办法让他们取得operator*返回的reference背后隐藏

的那个指针，这绝对导致内存泄漏。

(3)通过引用返回函数内定义的static对象

const Rational& operator* (const Rational& lhs, const Rational& rhs)
{
	static Rational result;
	result.n = lhs.n * rhs.n;
	result.d = lhs.d * rhs.d;
	return result;
}

现在不用担心对象在函数返回前被销毁，也不用担心内存泄漏，但考虑以下调用

Rational a, b, c, d;

if((a * b) == (c * d))
	...
else
	...

猜想怎么着？表达式((a * b) == (c * d))总是被核算为true, 不论a, b, c和d的值是什么。

由于返回static对象的引用，因此这两次调用的结果都是指向同一个对象，因此调用端看到的

永远是static Rational对象的"现值"。

*通过以上几类错误看出，试图令诸如operator*这样的函数返回reference，只是浪费时间。

一个"必须返回新对象"的函数的正确写法是：就让那个函数返回一个新对象， 对Rational

的operator*而言意味着以下写法：

inline const Rational operator* (const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
}

当然，你需得承受operator*返回值的构造和析构成本，然后长远来看那只是为了获得正确

行为而付出的一个小小代价。同时，如果你的编译器实施返回值优化，这种成本可被安全的

消除，你的程序将继续保持它们该有的行为，而执行起来又比预期的更快。
