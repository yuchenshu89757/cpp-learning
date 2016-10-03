Declare data members private

将成员变量声明为 private

1.起因：如果成员变量不是 public, 客户唯一能够访问对象的办法就是通过成员函数；如果public接口

内的每样东西都是函数，客户就不需要在打算访问class成员时迷惑地试着记住是否该使用小括号。

(1)使用函数可以让你对成员变量的处理有更精确的控制。

class AccessLevels{
	
public:

	...
	
	int  getReadOnly()const{ return readOnly;}
	
	void setReadWrite(int value){ readWrite = value;}
	
	int  getReadWrite()const{ return readWrite;}
	
	void setWriteOnly(int value){ writeOnly = value;}
	
private:

	int noAccess;		//对此int无任何访问动作；
	
	int readOnly;		//对此int做只读访问(read-only access)
	
	int readWrite;		//对此int做读写访问(read-write access)
	
	int writeOnly;		//对此int做惟写访问(write-only access)
	
};

I. 如此细微地划分访问控制颇有必要，因为许多成员变量应该被隐藏起来。每个成员变量都需要一个

getter函数和setter函数毕竟罕见。

II. 将成员变量隐藏在函数接口的背后，使得在外部接口不变的情况下，可以为"所有可能的实现"提供

弹性，而每一种内部实现方式对外部访问者而言是完全透明的。
