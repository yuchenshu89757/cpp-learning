Store newed objects in smart pointers in standalone statements

以独立语句将newed对象置入智能指针

1.起因：在"资源被创建"和"资源被转换为资源管理对象"两个时间点之间可能发生异常干扰。

2.举例：

假设我们有个函数用来揭示处理器的优先权，另一个函数用来在某动态分配所得的Widget上

进行某些带有优先权的处理：

int priority();

void processWidget(std::tr1::shared_ptr<Widget> pw, int priority);

由于谨记"以对象管理资源"的智慧名言，processWidget决定对其动态分配得来的Widget运用

智能指针（这里采用tr1::shared_ptr）

processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority());

虽然我们在此使用"对象管理式资源"，遗憾的是，上述调用却可能泄露资源。原因在于编译器

在产出一个 processWidget调用码之前，必须首先核算即将被传递的实参，于是在调用之前，

编译器必须创建代码，做以下几件事情：

(1) 调用 priority;

(2) 执行"new Widget";

(3) 调用 tr1::shared_ptr的构造函数；

而C++编译器以什么样的次序才来完成这些事情呢？答案是不知道，于是就可能出现这样次序：

(1) 执行"new Widget"

(2) 调用 priority;

(3) 调用 tr1::shared_ptr的构造函数；

现在，让我们设想一下，万一对priority的调用出现异常，会发生什么事？在此情况下"new Widget"

返回的指针将会遗失，因为它尚未被置入tr1::shared_ptr内，于是就引发了资源泄露。

3. 解决方案：使用分离语句，分别写出

(1) 创建Widget; 

(2) 将它置入一个智能指针内；

(3) 将智能指针传给processWidget;

std::tr1::shared_ptr<Widget> pw(new Widget);

processWidget(pw, priority());
