Differentiate between inheritance of interface and inheritance of implementation

区分接口继承和实现继承

1. 起因：表面上直截了当的public继承概念，经过更严密的检查之后，发现它由两部分组成：

函数接口继承和函数实现继承。

考虑一个展现绘图程序中各种几何形状的class继承体系：

class Shape{
public:
	//pure virtual member function
	virtual void draw()const = 0;
	//impure virtual member function
	virtual void error(const std::string& msg);
	//non-virtual member function
	int objectId()const;
	...
};

class Rectangle : public Shape{...};

class Ellipse : public Shape{...};

Shape是个抽象class, 它的pure virtual函数draw使它成为一个抽象class, 所有客户不能创建

Shape class的实体，只能创建其derived class的实体。尽管如此，Shape还是强烈影响了所有

以public形式继承它的derived class, 因为：

(1) 成员函数的接口总是会被继承

(2) 声明一个pure virtual函数的目的是为了让derived class只继承函数接口

pure virtual 函数有两个最突出的特性：

I. 必须被任何继承了它们的具象class重新声明；

II. 它们在抽象class中通常没有定义。

但是，我们可以为pure virtual函数提供定义, 调用它的唯一途径是明确指出其class名称。

(3) 声明简朴的impure virtual函数的目的，是让derived class继承该函数接口和缺省实现，

表达的含义是：你必须支持一个error函数，但如果你不想自己写一个，可以使用Shape class

提供的缺省版本。但允许impure virtual 函数同时指定函数声明和函数缺省行为，却有可能

造成危险。考虑如下三种飞机，A,B的飞行方式和C的不同

class Airplane{
public:
	virtual void fly(const Airport& destination) = 0;
	...
};

void Airplane::fly(const Airport& destination)
{
	缺省行为，将飞机飞至指定目的地
}

class ModelA : public Airplane{
public:
	virtual void fly(const Airport& destination)
	{
		Airplane::fly(destination);
	}
	...
};

class ModelB : public Airplane{
public:
	virtual void fly(const Airport& destination)
	{
		Airplane::fly(destination);
	}
	...
};

class ModelC : public Airplane{
public:
	virtual void fly(const Airport& destination);
	...
};

void ModelC::fly(const Airport& destination)
{
	将C型飞机飞至指定目的地
}

此时，纯虚函数fly被分割为两个基本要素：其声明部分表现的是接口，其定义部分则表现缺省行为；

(3) 声明non-virtual函数的目的是为了令derived classes继承函数的接口及一份强制性实现。

2. 总结

(1) pure virtual 函数只具体指定接口继承；

(2) impure virtual 函数具体指定接口继承及缺省实现；

(3) non-virtual 函数具体指定接口继承以及强制性实现继承。

