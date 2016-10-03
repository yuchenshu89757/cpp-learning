Never call virtual functions during construction or destruction

绝不在构造和析构过程中调用virtual函数

1.起因：(1) base class 构造函数的执行更早于derived class 构造函数，当

base class 构造函数执行时derived class 的成员尚未初始化。

(2) derived class 的析构函数的执行更早于base class 析构函数，当base 

class 析构函数执行时derived class 的成员已经被销毁。

(3) 在base class的构造和析构函数中调用virtual函数，只会出现两种结果：

I. 被调用的virtual函数在base class 中有定义，则直接调用base class 当前

定义的函数，而不是derived class中的函数。

II. 被调用的virtual函数在base class 中没有定义，连接器因找不到必要的

Transaction::logTransaction()实现代码而报错。

2. 举例: 假设有个class继承体系，用来模塑股市交易：

class Transaction{		//所有交易的base class
	
public:

	Transaction();
	
	virtual void logTransaction()const = 0;	//做出一份因类型不同而不同
											//的日志记录
	...
	
};

Transaction::Transaction(){
	
	...
	
	logTransaction();
	
}

class BuyTransaction : public Transaction{	//derived class
	
public:

	virtual void logTransaction()const;
	
	...
	
};

class SellTransaction : public Transaction{	//derived class
	
public:

	virtual void logTransaction();
	
	...
	
};

现在，当以下这行被执行时，

BuyTransaction b;

(1) 无疑会有一个BuyTransaction构造函数被调用，但Transaction构造函数一定会

更早被调用，Transaction构造函数的最后一行调用virtual函数logTransaction,

这时被调用的logTransaction是Transaction内的版本，不是BuyTransaction内的

版本，即使目前即将建立的对象类型是BuyTransaction。

(2) 更深层次的原因：在derived class 对象的 base class 构造期间，对象的类型

是 base class 而不是 derived class。不只virtual函数会被编译器解析至base class,

若使用运行期类型信息(例如dynamic_cast和typeid)，也会把对象视为base class类型。

3. 解决方案：(1) 确保构造函数和析构函数没有直接调用virtual函数，并且被它们调

用的所有函数也不能调用virtual函数。

(2) 将virtual函数改为non-virtual函数，并在构造期间，令derived class将必要的

构造信息给base class构造函数。像这样：

class Transaction{
	
public:

	explicit Transaction(const std::string& logInfo);
	
	void logTransaction(const std::string& logInfo);
	
	...
	
};

Transaction::Transaction(const std::string& logIngo)
{
	...
	logTransaction(logInfo);
}

class BuyTransaction : public Transaction{
	
public:

	BuyTransaction(parameters)
	: Transaction(createLogString(parameters))
	{...}
	
private:

	static std::string createLogString(parameters);
	
};
