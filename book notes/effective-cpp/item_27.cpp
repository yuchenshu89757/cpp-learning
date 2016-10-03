Minimize casting

尽量少做转型

1. 起因：C++规则的设计目标之一是，保证"类型错误"绝不可能发生。理论上如果你的程序很"干净地"通过

编译，就表示它并不企图在任何对象身上执行任何不安全、无意义、愚蠢荒谬的操作。不幸的是，转型破

坏了类型系统，那可能导致任何种类的麻烦，有些容易识别，有些非常隐晦。

2. C-style转型操作

(1) (T)expression	//将expression转型为T

(2) T(expression)	//将expression转型为T

两种形式并无差别，纯粹只是小括号的摆放位置不同而已，这两种形式俗称"旧式转型"。

3. C++转型操作

(1) const_cast: 唯一有能力将对象的常量性移除的C++-style转型操作符；

(2) dynamic_cast: 主要用来执行"安全向下转型", 也就是用来决定某对象是否归属继承关系中的某个类型。

它是唯一无法由旧式语法执行的动作，也是唯一可能耗费重大运行成本的转型操作。

(3) reinterpret_cast: 意图执行低级转型，实际动作（及结果）可能取决于编译器，这也表示它不可移植。

(4) static_cast: 强迫隐式转换。例如将non-const对象转换成const对象，或将int转为double等等，它也

可以执行上述多种转换的反向转换（const除外）。

4. 比较：旧式转型仍然合法，但新式转型较受欢迎。原因在于：

(1) 新式转型很容易在代码中被辨识出来，因而得以简化"找出类型系统在哪个地点被破坏"的过程；

(2) 各转型动作的目标愈窄化，编译器愈可能诊断出错误的运行。

5.举例

(1) 调用explicit构造函数，将对象传递给函数

class Widget{
public:
	explicit Widget(int size);
	...
};

void doSomeWork(const Widget& w);

doSomeWork(Widget(15));				//以一个int加上"函数风格"转型动作创建一个Widget

doSomeWork(static_cast<Widget>(15));//以一个int加上"C++风格"的转型动作创建一个Widget

始终使用新式转型更安全且合理。

(2)在derived class中调用base class的成员函数

class Window{
public:
	virtual void onResize(){...}
	...
};

class SpecialWindow : public Window{
public:
	virtual void onResize(){
		static_cast<Window>(*this).onResize();
		...	//这里进行SpecialWindow专属行为
	}
};

上述代码并非在当前对象身上调用Window::onResize之后又在该对象身上执行SpecialWindow专属动作，而

是在"当前对象之base class成分"的副本上调用Window::onResize，然后在当前对象身上执行SpecialWindow

专属动作。如果Window::onResize修改了对象内容，当前对象其实没被改动，改动的是副本。然而此时如果

SpecialWindow::onResize内也修改对象，当前对象真的会被改动。这使当前对象进入一种"伤残"状态：其

base class成分的更改没有落实，而derived class成分的更改倒是落实了。

*解决之道是拿掉转型动作，代之以你真正想说的话。你并不像哄骗编译器将*this视为一个base class对象，

你只是想调用base class版本的onResize函数，令它作用于当前对象身上。所以应该这么写：

class SpecialWindow : public Window{
public:
	virtual void onResize(){
		Window::onResize();	//调用Window::onReize作用于*this身上
		...
	}
};

(3)关于 dynamic_cast, 在探究 dynamic_cast 设计意涵之前，值得注意的是， dynamic_cast 的许多实现

版本执行速度相当慢。例如至少有一个很普遍的实现版本是基于"class名称之字符串比较"，如果你在四层深

的单继承体系内的某个对象身上执行"class名称之字符串比较", 那个版本所提供的每一次 dynamic_cast 可

能会耗用多达四次的strcmp调用，用以比较class名称。深度继承或多重继承的成本更高。

之所以需要 dynamic_cast, 通常是因为你想在一个你认定为derived class对象身上执行 derived class

操作函数，但你手上只有一个"指向base"的pointer或reference，你只能靠它们来处理对象。有两个一般性

的做法可以避免这个问题：

I. 使用容器并在其中存储指向derived class对象的指针(通常是智能指针)，如此便消除了"通过base class

接口处理对象"的需要。假设先前的Window/SpecialWindow继承体系中只有SpecialWindow才支持闪烁效果，试

着不要这样做：

class Window{...};

class SpecialWindow : public Window{
public:
	void blink();
	...
};

typedef std::vector<std::tr1::shared_ptr<Window>> VPW;

VPW winPtrs;

...

for(VPW::iterator it = winPtrs.begin(); it != winPtrs.end(); ++it)
{
	if(SpecialWindow* psw = dynamic_cast<SpecialWindow*>(it->get()))
		psw->blink();	
}

应该改成这样：

typedef std::vector<std::tr1::shared_ptr<SpecialWindow>> VPSW;

VPSW winPtrs;

...

for(VPSW::iterator it = winPtrs.begin(); it != winPtrs.end(); ++it)
	(*it)->blink();

当然，这种做法使你无法在同一个容器内存储指针"指向所有可能之各种Window派生类"。如果真要处理

多种窗口类型，你可能需要多个容器，它们都必须具备类型安全性。

II. 在base class内提供virtual函数做你对各个Window派生类做的事，可以实现通过base class接口处

理"所有可能之各种Window派生类"。例如，虽然只有 SpecialWindow可以闪烁，但或许将闪烁函数声明于

base class内并提供一份"什么也不做"的缺省实现代码是有意义的：

class Window{
public:
	virtual void blink(){}	
	...
};

class SpecialWindow : public Window{
public:
	virtual void blink(){...};
};

typedef std::vector<std::tr1::shared_ptr<Window>> VPW;

VPW winPtrs;

...

for(VPW::iterator it = winPtrs.begin(); it != winPtrs.end(); ++it)
	(*it)->blink();

不论哪一种写法："使用类型安全容器"或"将virtual函数往继承体系上方移动"，都并非放之四海而皆准，

但许多情况下它们提供了一个可行的 dynamic_cast 替代方案。

6. 总结：

(1) 如果可以，尽量避免转型，特别是在注重效率的代码中避免 dynamic_casts。如果有个设计需要

转型动作，试着发展无需转型的替代设计；

(2) 如果转型是必要的，试着将它隐藏于某个函数背后。客户随后可以调用该函数，而不需将转型放

进它们自己的代码内；

(3) 宁可使用C++ style转型，不要使用旧式转型。前者很容易辨识出来，而且也比较有着分门别类的职掌。
