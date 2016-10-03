#### 1. ����Ȩ������
��1��**public�̳�**
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
	void print(){//�������ڲ�����Ȩ�޲���
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //ok
		getB(); //ok
		c = 1;  //ok
		getC(); //ok
	}
};
//������������Ȩ�޲���
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //ok
d.getC();       //ok
```
�ɼ���public�̳�ģʽ�£�==�������ڲ�==���Է��ʻ����protected��public���Եĳ�Ա����==���������==ֻ�ܷ��ʻ���public���Եĳ�Ա��  
��2��**protected�̳�**
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
	void print(){//�������ڲ�����Ȩ�޲���
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
	void show(){//�����������ڲ�����Ȩ�޲���
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //ok
		getB(); //ok
		c = 1;  //ok
		getC(); //ok
	}
};
//������������Ȩ�޲���
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //cannot access public member declared in class 'Base'
d.getC();       //cannot access public member declared in class 'Base'
```
protected�̳�ģʽ�£�==�������ڲ�==���Է��ʻ����protected��public���Եĳ�Ա����==���������==�޷����ʻ����κγ�Ա�������������ڲ��Ի���ķ���Ȩ��ͬ���ĸ��ࡣ  
��3��**private�̳�**
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
	void print(){//�������ڲ�����Ȩ�޲���
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
	void show(){//�����������ڲ�����Ȩ�޲���
		a = 1;  //cannot access private member declared in class 'Base'
		getA(); //cannot access private member declared in class 'Base'
		b = 1;  //'b' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		getB(); //'getC' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		c = 1;  //'c' not accessible because 'Derived' uses 'private' to inherit from 'Base'
		getC(); //'getC' not accessible because 'Derived' uses 'private' to inherit from 'Base'
	}
};
//������������Ȩ�޲���
Derived d;
d.a = 1;        //cannot access private member declared in class 'Base'
d.getA();       //cannot access private member declared in class 'Base'
d.b = 1;        //cannot access protected member declared in class 'Base'
d.getB();       //cannot access protected member declared in class 'Base'
d.c = 1;        //cannot access public member declared in class 'Base'
d.getC();       //cannot access public member declared in class 'Base'

```
private�̳�ģʽ�£�==���������ڲ�==���Է��ʻ����protected��public���Եĳ�Ա����==���������==�޷����ʻ����κγ�Ա��++�����������ڲ��޷����ʻ�����κγ�Ա++��  

---
**�ܽ�**��==�������ּ̳�ģʽ�����������ڲ���Զ���Է��ʻ����protected��public���Եĳ�Ա��public�̳�ģʽ�£����������ֻ�ܷ��ʻ���public���Եĳ�Ա��protected��private�̳�ģʽ�£�����������޷����ʻ�����κγ�Ա��++private�̳�ģʽ�£�**������������ڲ�**�޷����ʻ�����κγ�Ա++��==
#### 2.Is-a / Has-a / Is-implemented-in-terms-of
��1��**public�̳���ģ��is-a��ϵ**
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
public�̳���ζ��"is-a"�Ĺ�ϵ��ÿ�����������ͬʱҲ�ǻ�����󣬷�֮������������˵���κλ�����Գ��ֵĵط�������Ҳһ�����Գ��֣����ϴ���ԭ�򣩡�  
��2��**����**  
- **����**������������֮���һ�ֹ�ϵ����ĳ�����͵Ķ����ں���������Ķ���ʱ���������ֹ�ϵ��������ζ��has-a����һ������ is-implemented-in-terms-of������ĳ��ʵ�ֳ����������Ϸ�����Ӧ�����ڵĶ����ˡ�������ĳЩ���֮�䣬���ֳ�has-a�Ĺ�ϵ������������ʵ���򣨻���������������ʵ��ϸ���ϵ��˹���Ʒ�������Ǳ���is-implemented-ni-terms-of�Ĺ�ϵ��

- **���ϱ���has-a**
```
class Address{...};
class PhoneNumber{...};
class Person{
public:
    ...
private:
    std::string name;       //�ϳɳɷ���
    Address address;        //ͬ��
    PhoneNumber voiceNumber;//ͬ��
    PhoneNumber faxNumber;  //ͬ��
};
```
����Person classʾ��has-a��ϵ��Person��һ�����ƣ�һ��סַ���Լ������ʹ������ʵ绰���롣���ǲ���˵������һ�����ơ��͡�����һ����ַ��������˵������һ�����ơ��͡�����һ����ַ����   

- **���ϱ���is-implemented-in-terms-of**
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
���������˼�ǡ�set����ɸ���list����ʵ�ֳ������������ǡ�set������һ��list���󡱡�set��Ա�����ɴ�������list�Լ���׼�����������ṩ�ļ�������ɡ� 

��3��**private�̳���ζ��is-implemented-in-terms-of**
```
class Person{...};
class Student : private Person{...};

void eat(const Person& p);

Person p;
Student s;

eat(p); //correct, p is a person
eat(s); //error
```
�����ϵ����ӿ��Կ�����private�̳в�����ζ��is-a��ϵ��

- ���classes֮����private�̳й�ϵ��1�������������Զ���һ��derived class����ת��Ϊһ��base class����2��==��private base class�̳ж��������г�Ա����derived class�ж�����private����==����ʹ������base class��ԭ����protected��public���ԡ�

- private�̳е���base class�е����г�Ա��derived class�ж���private�������ζ��derived class�Ķ����Լ���derived class���������඼�޷�ֱ�ӷ���derived class��base class���ֵĳ�Ա��Ҳ����ͨ��derived class����base class�Ľӿڱ������ˣ���ˣ�==private�̳���ζֻ��ʵ�ֲ��ֱ��̳У���ڲ���Ӧ��ȥ==��

- private�̳���ζis-implemented-in-terms-of, �����ϵ�����Ҳ��������==������ʹ�ø��ϣ���Ҫʱ��ʹ��private�̳�==����protected��Ա��/��virtual����ǣ������ʱ�����Ǿ͸ÿ���private�̳��ˡ�


#### 4.**EBO��empty base optimization: �հ׻������Ż���**
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
��1���հ׻���Empty������ʵ�����У����������empty����Ȼ���Ǵ�δӵ��non-static��Ա������ȴ�����ں�typedefs, enums, static��Ա��������non-virtual������ǰ���Ѿ����۹��� ��Щ�ɷ��ǲ�ռ��class���ڴ�ռ�ġ�  

��2���������ּ̳з�ʽ����������������Empty�����Ŀհ׻���ʱ������������ִ��EBO�� 

��3�����������ٸ��ϣ�������HasIntEmpty����private�̳У�HasIntPrivate��֮��ľ���ʱ�������׷��֣�private�̳�����EBO���ƶ����ӽ�ʡ�ռ䣬�����Ͼ�û��������������ʹ���ϳɷ���һ��Empty class�Ķ���