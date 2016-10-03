#### 1. 访问权限问题
（1）**public继承**
```
class Base{
public:
	void getC(){}
	int c;
protected:
	void getB(){}
	int b;
private:
	void getA(){}
	int a;
};
class Derived : public Base{
public:
	void print(){//派生类内部访问权限测试
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //ok
		getB(); //ok
		c = 1;  //ok
		getC(); //ok
	}
};
//派生类对象访问权限测试
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //ok
d.getC();       //ok
```
可见，public继承模式下，==派生类内部==可以访问基类的protected和public属性的成员；而==派生类对象==只能访问基类public属性的成员。  
（2）**protected继承**
```
class Base{
public:
	void getC(){}
	int c;
protected:
	void getB(){}
	int b;
private:
	void getA(){}
	int a;
};
class Derived : protected Base{
public:
	void print(){//派生类内部访问权限测试
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //ok
		getB(); //ok
		c = 1;  //ok
		getC(); //ok
	}
};
class SubDerived : public Derived{
public:
	void show(){//派生类子类内部访问权限测试
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //ok
		getB(); //ok
		c = 1;  //ok
		getC(); //ok
	}
};
//派生类对象访问权限测试
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //cannot access public member declared in class 'Base'
d.getC();       //cannot access public member declared in class 'Base'
```
protected继承模式下，==派生类内部==可以访问基类的protected和public属性的成员；而==派生类对象==无法访问基类任何成员；派生类子类内部对基类的访问权限同它的父类。  
（3）**private继承**
```
class Base{
public:
	void getC(){}
	int c;
protected:
	void getB(){}
	int b;
private:
	void getA(){}
	int a;
};
class Derived : private Base{
public:
	void print(){//派生类内部访问权限测试
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;
		getB();
		c = 1;
		getC();
	}
};
class SubDerived : public Derived{
public:
	void show(){//派生类子类内部访问权限测试
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //'b' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		getB(); //'getC' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		c = 1;  //'c' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		getC(); //'getC' not accessible because 'Derived' uses 'private' to inherit from 'Base'
	}
};
//派生类对象访问权限测试
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //cannot access public member declared in class 'Base'
d.getC();       //cannot access public member declared in class 'Base'

```
private继承模式下，==派生类类内部==可以访问基类的protected和public属性的成员；而==派生类对象==无法访问基类任何成员；++派生类子类内部无法访问基类的任何成员++。  

---
**总结**：==无论哪种继承模式，派生类类内部永远可以访问基类的protected和public属性的成员；public继承模式下，派生类对象只能访问基类public属性的成员，protected和private继承模式下，派生类对象无法访问基类的任何成员。++private继承模式下，**派生类的子类内部**无法访问基类的任何成员++。==
#### 2.Is-a / Has-a / Is-implemented-in-terms-of
（1）**public继承塑模出is-a关系**
```
class Person{};
class Student : public Person{}; 
void eat(const Person& p){}
void study(const Student& s){}

Person p;
Student s;

eat(p);     //correct, p is a person
eat(s);     //correct, s is a student as well as a person

study(p);   //error,   p is not a student
study(s);   //correct, s is a student
```
public继承意味着"is-a"的关系，每个派生类对象同时也是基类对象，反之不成立；或者说，任何基类可以出现的地方，子类也一定可以出现（里氏代换原则）。  
（2）**复合**  
- **概念**：复合是类型之间的一种关系，当某种类型的对象内含其他种类的对象时，便是这种关系。复合意味着has-a（有一个）或 is-implemented-in-terms-of（根据某物实现出）。当复合发生于应用域内的对象（人、汽车等某些事物）之间，表现出has-a的关系；当它发生于实现域（缓冲区、互斥器等实现细节上的人工制品）内则是表现is-implemented-ni-terms-of的关系。

- **复合表现has-a**
```
class Address{...};
class PhoneNumber{...};
class Person{
public:
    ...
private:
    std::string name;       //合成成分物
    Address address;        //同上
    PhoneNumber voiceNumber;//同上
    PhoneNumber faxNumber;  //同上
};
```
上述Person class示范has-a关系。Person有一个名称，一个住址，以及语音和传真两笔电话号码。我们不会说“人是一个名称”和“人是一个地址”，而会说“人有一个名称”和“人有一个地址”。   

- **复合表现is-implemented-in-terms-of**
```
template <typename T>
class Set{
public:
    bool member(const T& item)const;
    void insert(const T& item);
    void remove(const T& item);
    std::size_t size()const;
private:
    std::list<T> rep;
};
```
这里表达的意思是“set对象可根据list对象实现出来”，而不是“set对象有一个list对象”。set成员函数可大量依赖list以及标准库其他部分提供的技能来完成。 

（3）**private继承意味着is-implemented-in-terms-of**
```
class Person{...};
class Student : private Person{...};

void eat(const Person& p);

Person p;
Student s;

eat(p); //correct, p is a person
eat(s); //error
```
从以上的例子可以看出，private继承并不意味着is-a关系。

- 如果classes之间是private继承关系：1）编译器不会自动将一个derived class对象转换为一个base class对象；2）==由private base class继承而来的所有成员，在derived class中都会变成private属性==，纵使它们在base class中原本是protected或public属性。

- private继承导致base class中的所有成员在derived class中都是private，这就意味着derived class的对象以及由derived class派生的子类都无法直接访问derived class中base class部分的成员，也就是通过derived class访问base class的接口被屏蔽了，因此，==private继承意味只有实现部分被继承，借口部分应略去==。

- private继承意味is-implemented-in-terms-of, 而复合的意义也是这样。==尽可能使用复合，必要时才使用private继承==。当protected成员或/和virtual函数牵扯进来时，我们就该考虑private继承了。


#### 4.**EBO（empty base optimization: 空白基类最优化）**
```
class Empty{};

class HasIntPublic : public Empty{
private:
    int x;
};

class HasIntProtected : protected Empty{
private:
    int x;
};

class HasIntPrivate : private Empty{
private:
    int x;
};

class HasIntEmpty{
private:
    int x;
    Empty e;
};

cout << "sizeof(Empty) = " << sizeof(Empty) << endl;                    //print 1
cout << "sizeof(HasIntPublic) = " << sizeof(HasIntPublic) << endl;      //print 4
cout << "sizeof(HasIntProtected) = " << sizeof(HasIntProtected) << endl;//print 4
cout << "sizeof(HasIntPrivate) = " << sizeof(HasIntPrivate) << endl;    //print 4
cout << "sizeof(HasIntEmpty) = " << sizeof(HasIntEmpty) << endl;        //print 8
```
（1）空白基类Empty，在现实生活中，并不真的是empty，虽然它们从未拥有non-static成员变量，却往往内含typedefs, enums, static成员变量，或non-virtual函数。前面已经讨论过， 这些成分是不占用class的内存空间的。  

（2）无论哪种继承方式，当基类是类似于Empty这样的空白基类时，编译器都会执行EBO。 

（3）当我们面临复合（类似于HasIntEmpty）和private继承（HasIntPrivate）之间的抉择时，很容易发现，private继承由于EBO机制而更加节省空间，而复合就没法做到这样，即使复合成分是一个Empty class的对象。