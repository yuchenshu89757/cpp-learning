Treat class design as type design

设计class犹如设计type

1.起因：C++就像其他OOP语言一样，当定义一个新class，也就定义了一个新type。设计优秀的classes

是一项艰巨的任务，好的types有自然的语法，直观的语义，以及一或多个实现品。

2.设计规范：

(1) 新type的对象应该如何被创建和销毁？这涉及到以下几个函数的设计

I. class 的构造和析构函数；

II. 内存的分配和释放函数(new/delete, new[]/delete[]);

(2) 对象的初始化和对象的赋值该有什么样的差别？这涉及到构造函数和赋值操作符的行为，以及期间

的差异。最重要的是别混淆了"初始化"和"赋值"，因为它们对应于不同的函数调用。

(3) 新type的对象如果被pass by value，意味着什么？copy构造函数用来定义一个type的pass-by-value

该如何实现。

(4) 什么是新type的"合法值"？对class的成员变量而言，通常只有某些数值集是有效的，那些数值集决

定了你的class必须维护的约束条件，也就是决定了你的成员函数(特别是构造函数、赋值操作符和所谓

"setter"函数)必须进行的错误检查工作。

(5)你的新type需要配合某个继承图系吗？

I. 如果你的class继承自某些既有的classes, 你就受到那些classes的设计的束缚，特别是受到 virtual

或 non-virtual 的影响；

II. 如果允许其他classes继承你的class, 那会影响你所声明的函数，尤其是析构函数，是否为 virtual

(6)你的新type需要什么样的转换？

I. 如果允许类型T1之物被隐式转换为类型T2之物，就必须在class T1内写一个类型转换函数(operator T2)

或在class T2内写一个non-explicit-one-argument(可被单一实参调用)的构造函数；

II. 如果允许explicit构造函数存在，就得写专门负责执行转换的函数，且不得为类型转换操作符或可被单

一实参调用的构造函数。

(7)什么样的操作符和函数对此新type而言是合理的？这个问题的答案决定你将为你的class声明哪些函数。

其中某些该是member函数，某些则否。

(8)什么样的标准函数应该驳回？那些正是你必须声明为 private 者。

(9)谁该取用新type的成员？这个提问可以帮你决定以下两件事情：

I. 哪个成员为 public, 哪个成员为 protected, 哪个为 private;

II. 哪一个classes和/或functions应该是friends, 以及它们嵌套于另一个之内是否合理；

(10)什么是新type的"未声明接口"？它对效率、异常安全性以及资源运用提供何种保证？你在这些方面提

供的保证将为你的class实现代码加上相应的约束条件。

(11)你的新type有多么一般化？或许你其实并非定义一个新type，而是定义一整个types家族，果真如此

你就不该定义一个class, 而是定义一个新的class template。 


