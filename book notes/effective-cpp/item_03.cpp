Use const whenever possible

1. 令函数返回一个常量值，往往可以降低因客户错误而造成的意外，而又不至于放弃安全性和高效性

例如，有理数的operator*声明式：

class Rational {...};

const Rational operator* (const Rational& lhs, const Rational& rhs);

这样做可以避免一些无谓的赋值操作，例如

Rational a, b, c;

(a * b) = c;

if(a * b = c){}

2. const成员函数，将const实施于成员函数的目的，是为了确认该成员函数可作用于const对象上

（1）两个成员函数如果只是常量性不同，可以被重载，例如

class TextBlock{

public:

...

const char& operator[](std::size_t position)const
{
    return text[position];
}

char& operator[](std::size_t position)
{
    return text[position];
}

private:

std::string text;

}

在使用过程中

TextBlock tb("Hello");

const TextBlock ctb("World");

std::cout << tb[0]; //调用non-const TextBlock::operator[]

std::cout << ctb[0];//调用const TextBlock::operator[]

如果TextBlock的operator[]不仅仅返回一个reference，也执行边界检查、志记访问信息等，把所有的这些同时放进const

和non-const operator[]中，就会导致大量的代码重复，而且不利于维护。令non-constoperator[]调用其const兄弟是一个

避免代码重复的安全做法

class TextBlock{

public:

const char& operator[] (std::size_t position)const
{
    ...
    ...
    ...
    return text[position];
}

char& operator[](std::size_t position)
{
    return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
}
}
