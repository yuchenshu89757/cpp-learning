Strive for exception-safe code

为"异常安全"而努力是值得的

1. 假设有个class用来表现夹带背景图案的GUI菜单，这个class希望用于多线程环境，所以它有个

互斥器(mutex)作为并发控制之用：

class PrettyMenu{
public:
	...
	//改变背景图像
	void changeBackground(std::istream& imgSrc);
	...
private:
	//互斥器
	Mutex mutex;
	//目前的背景图像
	Image* bgImage;
	//背景图像被改变的次数
	int imageChanges;
};

下面是PrettyMenu的changeBackground函数的一个可能实现：

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	lock(&mutex);					//取得互斥器
	delete bgImage;					//摆脱旧的背景图像
	++imageChanges;					//修改图像变更次数
	bgImage = new Image(imgSrc);	//安装新的背景图像
	unlock(&mutex);					//释放互斥器
}

从"异常安全性"的观点来看，这个函数很糟糕。"异常安全"有两个条件，而这个函数没有满足其中

任何一个条件。当异常被抛出时，带有异常安全性的函数会：

(1) 不泄漏任何资源。上述代码没有做到这一点，因为一旦"new Image(imgSrc)"导致异常，对unlock

的调用就绝不会执行，于是互斥器就永远被锁住了。解决这个问题很容易，直接用对象管理资源即可：

class Lock{
public:
	explicit Lock(Mutex* pm):mutexPtr(pm)
	{
		lock(mutexPtr);		//获得资源
	}
	~Lock()
	{
		unlock(mutexPtr);
	}
private:
	Mutex* mutexPtr;		//释放资源
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	Lock m1(&mutex);
	delete bgImage;
	++imageChanges;
	bgImage = new Image(imgSrc);
}

关于"资源管理类"如Lock者，一个最棒的事情是，它们通常使函数更短。更少的代码意味着出错机

会较少，而且一旦有所改动，被误解的机会也比较少。

(2) 不允许数据败坏。如果"new Image(imgSrc)"抛出异常，bgImage就是指向一个已被删除的对象，

imageChanges也已被累加，而其实并没有新的图像被成功安装起来。关于数据败坏，异常安全函数

提供以下三个保证之一：

I. 基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或数据结

构会因此而败坏，所有对象都处于一种内部前后一致的状态，然而程序的现实状态恐怕不可预料。

II. 强烈保证：如果异常被抛出，程序状态不改变。调用这样的函数须有这样的认知：如果函数成功，

就是完全成功，如果函数失败，程序会回滚到"调用函数之前"的状态。与此形成对比的是，如果调用

一个只提供基本承诺的函数，而真的出现异常，程序可能处于任何状态，只要那是个合法的状态。

III. 不抛掷保证：承诺绝不抛出异常，因为它们总是能够完成它们原先承诺的功能。作用于内置类

型身上的所有操作都提供nothrow保证，这是异常安全码中一个必不可少的关键基础材料。

	异常安全码必须提供上述三种保证之一，否则就不具备异常安全性。因此，我们的抉择是，该为我
	
们所写的每一个函数提供提供哪一种保证。一般而言你应该会想提供可实施之最强烈保证。从异常安全

性的观点视之，nothrow函数很棒，但我们很难再C part of C++领域中完全没有调用任何一个可能抛出

异常的函数。对大部分函数而言，抉择往往落在基本保证和强烈保证之间。

	对 changeBackground而言，提供强烈保证几乎不困难。首先改变PrettyMenu的bgImage成员变量的
	
类型，从一个类型为Image*的内置指针改为一个"用于资源管理"的智能指针，并重新排列changeBackground

内的语句次序，使得在更换图像之后才累加imageChanges:

class PrettyMenu{
	...
	std::tr1::shared_ptr<Image> bgImage;
	...
};

void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	Lock m1(&mutex);
	bgImage.reset(new Image(imgSrc));	//以"new Image"的执行结果设定bgImage内部指针
	++imageChanges;
}

注意，这里不再需要手动delete旧图像，因为这个动作已经由智能指针内部处理掉了。此外，删除动作

只发生在新图像被成功创建之后。以智能指针管理资源再次缩减了changeBackground的长度。

	然而这两个改变仍旧不能百分之百保证异常安全，如果Image的构造函数抛出异常，有可能输入流
	
的读取记号已被移走，而这样的搬移对程序其余部分是一种可见的状态改变。所以 changeBackground

在解决这个问题之前只提供基本的异常安全保证。有个一般化的设计策略很典型地会导致强烈保证，

这个策略被称为copy-and-swap, 原则很简单：
	
(1) 为你打算修改的对象做出一份副本;

(2) 在副本身上做一切必要修改。若有任何修改动作抛出异常，原对象保持未改变状态。

(3) 待所有改变都成功后，再将修改过的那个副本和原对象在一个不抛出异常的操作中置换。

	实现上，通常使将所有"隶属对象的数据"从原对象放进另一个对象内，然后赋予原对象一个指针，
	
指向那个所谓的实现对象，这种手法常被称为pimpl idiom，对PrettyMenu而言，典型写法如下：

struct PMImpl{
	std::tr1::shared_ptr<Image> bgImage;
	int imageChanges;
};

class PrettyMenu{
	...
private:
	Mutex mutex;
	std::tr1::shared_ptr<PMImpl> pImpl;
};
	
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
	using std::swap;
	Lock(&mutex);
	std::tr1::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));
	pNew->bgImage.reset(new Image(imgSrc));
	++pNew->imageChanges;
	swap(pImpl, pNew);
}
	
2. 总结

(1) 异常安全函数即使发生异常也不会泄漏资源或允许任何数据结构败坏。这样的函数区分为三种

可能的保证：基本型、强烈性、不抛异常型；

(2) "强烈保证"往往能够以copy-and-swap实现出来，但"强烈保证"并非对所有函数都可实现或具备

现实意义；

(3) 函数提供的"异常安全保证"通常最高只等于其所调用之各个函数的"异常安全保证"中的最弱者。
