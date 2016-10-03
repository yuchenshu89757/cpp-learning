Prevent exceptions from leaving destructors

别让异常逃离析构函数

1. 起因：析构函数抛出异常会导致程序出现不明确行为或结束执行。

2. (1)析构函数绝对不要吐出异常。如果一个被析构函数调用的函数可能抛出异常，析构

函数应该捕获任何异常，然后吞下它们（不传播）或结束程序。

例如，对于一个负责数据库连接的类：

class DBConn{
	
public:

	...
	
	~DBConn()
	{
		db.close();
	}
	
private:

	DBConnection db;
	
};

如果析构函数调用close出现异常，通常有两种解决办法：

I. 如果抛出异常就结束程序。通常通过调用abort完成：

DBConn::~DBConn()
{	
	try{db.close()}
	catch(...){
		制作运行记录，记下对close的调用失败;
		std::abort();
	}
}

II. 吞下调用close而发生的异常

DBConn::~DBConn()
{
	try{db.close();}
	catch(...){
		制作运行记录，记下对close的调用失败;
		...
	}
}

(2)如果客户需要对某个操作函数运行期间抛出的异常做出反应，那么class应该提供一个

普通函数（而非在析构函数中）执行该操作。

例如,对于上述DBConn类，我们可以重新设计：

class DBConn{
	
public:

	...
	
	void close()	//供客户调用的新函数
	{
		db.close();
		closed = true;
	}
	
	~DBConn()
	{
		if(!closed){
			try{
				db.close();
			}
			catch(...){
				制作运行记录，记下对close的调用失败;
				...
			}
		}
	}
	
private:

DBConnection db;

bool closed;

};

这样就给客户提供了一个处理错误的机会，如果因为客户没有调用而导致异常，客户没立场抱怨。
