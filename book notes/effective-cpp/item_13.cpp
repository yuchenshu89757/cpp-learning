Use object to manage resources

以对象管理资源

1.起因：C++许多资源被动态分配于heap内而后被用于单一区块或函数内，它们应该在控制流离开

那个区块或函数时被释放，否则就容易导致内存泄漏。

2.举例：假设我们使用一个用来模塑投资行为的程序库，其中各式各样的投资类型继承自一个root

class Investment:

class Investment{...};	//"投资类型"继承体系中的root class

进一步假设，这个程序库系通过一个工厂函数供应我们某特定的Investment对象：

Investment* createInvestment(); //返回指针，指向Investment继承体系内的动态分配对象

createInvestment的调用端使用了函数返回的对象后，有责任删除之，现在考虑有个f函数履行

了这个责任：

void f()
{
	Investment* pInv = createInvestment();
	...
	delete pInv;
}

这看起来很妥当，但若干情况下f可能无法删除它得自createInvestment的投资对象：

(1)"..."区域内的一个过早的 return 语句;

(2)对createInvestment的使用及 delete 动作位于某循环内，而该循环由于某个 continue

或 goto 语句过早退出；

(3)"..."区域内的语句抛出异常，则 delete 语句将不会被执行；

*为确保createInvestment返回的资源总是被释放，我们需要将资源放进对象内，当控制流离开

f, 该对象的析构函数会自动释放那些资源。

3.解决方案：使用智能指针auto_ptr，shared_ptr管理资源对象。

(1)auto_ptr被销毁时会自动删除它所指之物，它有一个不同寻常的性质：若通过copy构造函数

或copy assignment操作符复制它们，它们会变成 NULL, 而复制所得的指针将获得资源的唯一

拥有权，这就保证了一个对象同时只被一个auto_ptr指向。

(2)shared_ptr是引用计数型智慧指针，它会持续追踪共有多少对象指向某笔资源，并在无人指

向它时自动删除该资源。它有一个缺点是无法打破环状引用，例如两个其实已经没被使用的对象

彼此互指，因而好像还处在"被使用"状态。

(3)auto_ptr和shared_ptr两者都在其析构函数内做delete而不是delete[]动作，那意味着在动

态分配而得的array身上使用auto_ptr或tr1::shared_ptr是个馊主意。

因此，f可以改写为：

f()
{
	std::auto_ptr<Investment> pInv(createInvestment());
	...
}

或者

f()
{
	std::tr1::shared_ptr<Investment> pInv(createInvestment());
	...
}
