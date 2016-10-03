Use the form in corresponding uses of new and delete

成对使用 new 和 delete 时要采取相同形式

1.起因：C++用于动态分配内存的运算符是 new/delete, new[]/delete[]，由 new 创建的对象必须

通过调用 delete 来销毁，而 new[] 创建的对象必须调用 delete[] 来销毁。原因在于：

(1) new/delete 用于单个对象的动态创建和销毁, new[]/delete[]用于数组的动态创建和销毁；

(2) 当使用 new 动态生成一个对象时，有两件事情发生：

I. 调用 operator new 申请内存，类似于C语言中的malloc函数，空间大小为 sizeof(T)；

II. 调用 placement new 在申请的内存空间上调用对象的构造函数创建对象；

(3) 当使用 delete 销毁一个由 new 创建的对象时，也有两件事情发生：

I. 调用对象的析构函数销毁对象；

II. 调用 operator delete 删除内存空间

(4) 当我们使用 new[] 来创建对象时，实际上是创建了N个类型为T的对象，每一个对象在创建时

都会调用构造函数，而 delete[] 代表销毁多个对象，每个对象都会调用析构函数；

(5) 在实际应用中，到底应该调用 delete 还是 delete[]，这取决于即将被删除的指针，所指的

是单一对象或对象数组，因为单一对象的内存布局一般而言不同于数组的内存布局。

(6) 当对一个指针使用 delete 时, 系统便认定指针指向单一对象，而使用 delete[]时，系统便

认定指针指向一个数组。

2. 解决方案：如果你调用 new 时使用[],必须在对应调用 delete 时也使用[]。如果你调用 new

时，没有使用[],那么也不该在对应调用 delete 时使用[]。

*对于使用 typedef 的程序需要格外小心，例如

typedef std::string AddressLines[4];	//每个人的地址有4行，每行一个string

由于AddressLines是个数组，如果这样使用 new:

std::string* pal = new AddressLines;

那就必须匹配"数组形式"的 delete:

delete pal;		//行为未定义

delete []pal;	//正确
