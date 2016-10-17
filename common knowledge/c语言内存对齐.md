#C语言中内存对齐的问题
####1. 内存对齐的原理
处理器并不是按字节块来存取内存的，它一般会以双字节,四字节,8字节,16字节甚至32字节为单位来存取内存，我们将上述这些存取单位称为**内存存取粒度**。以四字节的存取粒度为例，假设现在要存取4字节，对于对齐的内存，处理器只需一次访存，而对于非对齐的内存，处理器需要两次访存，剔除不需要的数据，合并两块数据放入寄存器，这需要做很多工作，有些编译器根本就不支持这种非对齐内存访问。
####2. 内存对齐规则
#####(1) 结构体内的元素对齐规则
结构体中的元素按照定义顺序依次放入内存，从结构体存储的首地址开始，**每个元素开始放置的位置一定是自身宽度的整数倍**（以结构体变量首地址为0计算）。
```
struct X{
	char a;
	int b;
	double c;
}s;

int main(void)
{
	cout << sizeof(s) << endl;
	cout << sizeof(s.a) << endl;
	cout << sizeof(s.b) << endl;
	cout << sizeof(s.c) << endl;
	return 0;
}
```
上述代码的输出结果依次为：16, 1, 4, 8。可以看到s占用的内存大小并不等于其成员占用内存的之和，这里面就存在内存对齐的问题。我们来仔细分析上例:  
> * 字符型变量a占1byte，开始存储的位置是0,1,2,...，因此a被存入第0个byte开始的位置，占据0~1byte；
> * 整形变量b占4bytes，开始存储的位置是0,4,8,12,...，由于0byte的位置已经被占用，因此b被存入第4个byte开始的位置，占用4~7byte；
> * 双精度浮点型变量c占8bytes，开始存储的位置是0,8,16,24,...，由于0byte的位置已被占用，因此c被存入第8个byte开始的位置，占用8~15byte；

#####(2) 结构体自身的对齐规则
结构体内元素对齐之后，检查计算出的存储单元是都为所有元素中**最宽的元素的长度的整数倍**，是，则结束；否则，补齐为它的整数倍。
```
struct X{
	char a;
	double b;
	int c;
}s;

int main(void)
{
	cout << sizeof(s) << endl;
	cout << sizeof(s.a) << endl;
	cout << sizeof(s.b) << endl;
	cout << sizeof(s.c) << endl;
	return 0;
}
```
上述例子是在第一个例子的基础上交换了结构体b和c成员的位置，打印的结果为: 24,1,8,4。我们还是给出详细分析过程：
> * 字符型变量a占1byte，开始存储的位置是0,1,2,...，因此a被存入第0个byte开始的位置，占据0byte；
> * 双精度浮点型变量b占8bytes，开始存储的位置是0,8,16,24,...，由于0byte的位置已被占用，因此b被存入第8个byte开始的位置，占用8~15byte；
> * 整形变量c占4bytes，开始存储的位置是0,4,8,12,16..，由于0,4,8,12byte的位置都占用，因此c被存入第16个byte开始的位置，占用16~19byte；
> * 检查存储长度为20bytes，不是最宽元素长度8的整数倍，因此将它补齐到8的整数倍，也就是24bytes。

#####(3) 嵌套结构体/共用体/数组的对齐规则
含有构造数据类型（数组、结构体和联合体）的情况，首先要明确的是计算存储空间时要把构造体看成一个整体来为其开辟存储空间；其次要明确的是在**最后补齐时是按照所有元素中的基本数据类型的最长宽度来补齐**的，也就是说虽然要把构造体看作整体，但在补齐的时候并不会按照所含结构体所占存储空间的长度来补齐的（即使它可能是最长的）。

```
struct X{
	char a;
	short b;
	double c;
	int d;
};

union U{
	char a[4];
	short b[2];
	int c;
};

struct Y{
	char a;
	short b[5];
	double c;
	char d;
	X e;
	U f;
}s;

int main(void)
{
	cout << "sizeof(s)   = " << sizeof(s) << endl;
	cout << "sizeof(s.a) = " << sizeof(s.a) << endl;
	cout << "sizeof(s.b) = " << sizeof(s.b) << endl;
	cout << "sizeof(s.c) = " << sizeof(s.c) << endl;
	cout << "sizeof(s.d) = " << sizeof(s.d) << endl;
	cout << "sizeof(s.e) = " << sizeof(s.e) << endl;
	cout << "sizeof(s.f) = " << sizeof(s.f) << endl;

	printf("&s   = 0x%p\n", &s);
	printf("&s.a = 0x%p\n", &s.a);
	printf("&s.b = 0x%p\n", &s.b);
	printf("&s.c = 0x%p\n", &s.c);
	printf("&s.d = 0x%p\n", &s.d);
	printf("&s.e = 0x%p\n", &s.e);
	printf("&s.f = 0x%p\n", &s.f);
	return 0;
}
```
本例可能稍微复杂一点，为了便于分析，我们把每个成员的地址也打印出来，打印结果：
```
sizeof(s)   = 64
sizeof(s.a) = 1
sizeof(s.b) = 10
sizeof(s.c) = 8
sizeof(s.d) = 1
sizeof(s.e) = 24
sizeof(s.f) = 4
&s   = 0x0040d040
&s.a = 0x0040d040
&s.b = 0x0040d042
&s.c = 0x0040d050
&s.d = 0x0040d058
&s.e = 0x0040d060
&s.f = 0x0040d078
```
> * 字符型变量a占1byte，开始存储的位置为0,1,2,3,...，因此a被存入第0个byte开始的位置，占据0byte；
> * 短整型数组变量b占10bytes，但其元素基本类型是short，占2bytes，开始存储存储的位置为0,2,4,6,...，因此b被存入第2个byte开始的位置，占据2~11byte；
> * 双精度浮点型变量c占8bytes，开始存储的位置是0,8,16,24,...，因此c被存入第16个byte开始的位置，占据0~23byte；
> * 字符型变量d占1byte，开始存储的位置为0,1,2,3,...，因此a被存入第24个byte开始的位置，占据0byte；
> * 结构体X型变量e占24bytes，但是其所含的成员中，基本数据类型最宽的为8bytes，因此e开始存储的位置是0,8,16,24,32...；最终e被存入第32个byte开始的位置，占据32~55byte；
> * 共用体变量f占4bytes，开始存储的位置为0,4,8,12,16,...，因此f被存入第56个byte开始的位置，占据56~59byte；
> * 检查存储长度为60bytes,不是最宽元素长度8的整数倍，因此将它补齐到8的整数倍，也就是64bytes。