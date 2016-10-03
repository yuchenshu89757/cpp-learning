Declare destructors virtual in polymorphic base classes

为多态基类声明virtual析构函数

1. 起因：C++明确指出，当derived class对象经由一个base class指针被删除，而该base class

带着一个non-virtual析构函数，其结果未有定义，实际执行时通常发生的是对象的derived class

成分没有被销毁。

2. (1)带多态性质的base classes应该声明一个virtual析构函数。这种规则只适用于带多态性质的

base classes。如果希望该base class仅仅只作为接口，可以将该类的析构函数声明为pure 
	
virtual函数，纯虚函数会导致abstract class，抽象class是不能被实体化的class。
	
class Animal{
		
public:
	
	virtual ~Animal() = 0;	//声明pure virtual析构函数
		
}
	
由于derived class在销毁的过程中，会由深至浅依次调用base classes的析构函数，因此还必须
	
为pure virtual函数提供一份定义：
	
Animal::~Animal(){}

 (2)带有任何virtual函数的classes应该拥有一个virtual析构函数。
   
 (3)除此之外，如果classes的设计目的不是作为base classes使用，或是为了多态，就不该声明
   
virtual 析构函数。
