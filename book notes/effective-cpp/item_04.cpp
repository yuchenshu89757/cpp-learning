Make sure that objects are initialized before they are used

永远在使用对象之前先将它初始化

1. 对于无任何成员的内置类型，需要程序员手工完成初始化

int x = 0;    //对int进行手工初始化

const char* text = "A C-Style string";    //对指针进行手工初始化

double d;

cin>>d;       //以读取input stream的方式完成初始化

2. 对于内置类型以外的任何其他东西，初始化责任落在构造函数身上，规则很简单：确保每一个构造函数都将对象的每一个成员初始化。

这句话的言外之意是，一个类可能有多个构造函数，每个构造函数都有责任将该类的每一个成员初始化，包括默认构造函数。

class PhoneNumber{...};

class ABEntry{

public:

	ABEntry(const std::string& name, const std::string& address,
	
			const std::list<PhoneNumber>& phones);
			
private:

	std::string theName;
	
	std::string theAddress;
	
	std::list<PhoneNumber> thePhones;
	
	int numTimesConsulted;
};

//自定义构造函数

ABEntry::ABEntry(const std::string& name, const std::string& address

				const std::list<PhoneNumber>& phones):theName(name),
				
				theAddress(address), thePhones(phones), numTimesConsulted(0){}

//默认构造函数
				
ABEntry::ABEntry():theName(), theAddress(), thePhones(),numTimesConsulted(0){}

备注：上述初始化过程采用的是成员初值列（member initialization list）的做法，这种

方法通常比在构造函数体中采用赋值初始化效率要高，如果成员变量是const或reference，

它们就只能通过成员初值列来初始化，不能通过赋值初始化。因此，最简单的做法就是，永远

使用成员初值列。

C++成员初始化次序：（1）base classes更早于其derived classes被初始化；（2）class的成员

变量总是以其声明次序被初始化，即便它们在成员初值列中以不同的次序出现

3. 不同编译单元内定义之non-local static 对象的初始化次序

（1）函数内的static对象成为local static对象，其他static对象成为non-static对象

（2）static对象在main()结束时自动调用析构函数销毁

例如，假设有一个FileSystem class,

class FileSystem{
	
public:

	...
	
	std::size_t numDisks()const;	//众多成员函数之一
	
	...
};

extern FileSystem tfs;		//预备给客户使用的对象，tfs代表theFileSystem

如果客户在theFileSystem对象构造完成前就是用它，会发生很严重的错误

class Directory{
	
public:

	Directory(params);
	
	...
	
};

Directory::Directory(params)
{
	
	...
	
	std::size_t disks = tfs.numDisks();	//使用tfs对象
	
	...
	
}

进一步假设，客户决定创建一个Directory对象，用来放置临时文件

Directory tempDir(params);

现在问题出来了，除非tfs先于tempDir完成初始化，否则tempDir的构造函数会用到尚未初始化的tfs,

但tfs和tempDir是不同的人在不同的时间于不同的源码文件建立起来的，它们是定义于不同编译单元

内的non-local static 对象，如何能够确定tfs会在tempDir之前先被初始化？

（3）解决方案：将每个non-local static 对象搬到自己的专属函数内（该对象在此函数内被声明为

				static），这些函数返回一个reference指向它所含的对象。

（4）基于的原理：C++保证，函数内的local static 对象会在"该函数被调用期间""首次遇上该对象之定义式"
				
				 时被初始化。所以以"函数调用"替换"直接访问non-local static对象"，就保证了所获得的
				 
				 那个reference指向了一个历经初始化的对象，更重要的是，如果从未调用non-local static
				 
				 对象的"仿真函数"，就绝对不会引发构造和析构成本。
				 
此技术实施于tfs和tempDir身上，结果如下：

class FileSystem{...};		//同前

FileSystem& tfs()			//此函数用来替换tfs对象；它在FileSystem class中可能是个static
{
	
	static FileSystem fs;	//定义并初始化一个local static对象
	
	return fs;				//返回一个reference指向上述对象
	
}

class Directory{...};		//同前

Directory::Directory(params)
{
	
	...
	
	std::size_t disks = tfs().numDisks();
	
	...
	
}

Directory& tempDir()		//这个函数用来替换tempDir对象，它在Directory class中可能是个static
{
	
	static Directory td;	//定义并初始化local static对象
	
	return td;				//返回一个reference指向上述对象
	
}
