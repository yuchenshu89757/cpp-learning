Postpone variable definations as long as possible

尽可能延后变量定义式的出现时间

1.起因：只要你定义了一个变量而其类型带有一个构造函数或析构函数，那么当程序的控制流到达这个

变量定义式时，你便得承受构造成本；当这个变量离开其作用域时，你便得承受析构成本。即使这个变

量最终并未被使用，仍需耗费这些成本，所以你应该尽可能避免这种情形。

2.举例

(1)考虑下面这个函数，它计算通行密码的加密版本而后返回，前提是密码够长。但如果密码太短，函数

会丢出一个异常，类型为logic_error:

//这个函数过早定义变量"encrypted"
std::string encryptPassword(const std::string& password)
{
	using namespace std;
	string encrypted;
	if(password.length() < MinimumPasswordLength)
		throw logic_error("Password is too short");
	...		//必要动作，俾能将一个加密后的密码置入变量encrypted内
	return encrypted;
} 

对象encrypted在此函数中并非完全未被使用，但如果有个异常被丢出，它就真的没被使用。也就是如果

函数encryptPassword丢出异常，你仍得付出encrypted的构造成本和析构成本。所以最后延后encrypted

的定义式，直到确实需要它：

//这个函数延后"encrypted"的定义，直到真正需要它
std::string encryptPassword(const std::string& password)
{
	using namespace std;
	if(password.length() < MinimumPasswordLength)
		throw logic_error("Password is too short");
	string encrypted;
	...		//必要动作，俾能将一个加密后的密码置入变量encrypted内
	return encrypted;
}

但这段代码仍然不够秾纤合度，因为encrypted虽获定义却无任何实参作为初值。这意味调用的是其default

构造函数。许多时候你该对对象做的第一次事就是给它个值，通常是通过一个赋值动作达成。但通过default

构造函数构造出一个对象然后对它赋值往往比直接在构造时指定初值的效率差。举个例子，假设

encryptPassword的加密部分在以下函数中进行：

void encrypt(std::string& s);	//在其中的适当地点对s加密

于是encryptPassword可实现如下，虽然还不算是最好的做法：

//这个函数延后"encrypted"的定义式，直到确实需要它
//但此函数仍然有着不该有的效率低落
std::string encryptPassword(const std::string& password)
{
	...						//检查length,如前
	std::string encrypted;	//default-construct encrypted
	encrypted = password;	//赋值给encrypted
	encrypt(encrypted);
	return encrypted;
}

更受欢迎的做法是以password作为encrypted的初值，跳过毫无意义的default构造过程：

//终于，这是定义并初始化encrypted的最佳做法
std::string encryptPassword(const std::string& password)
{
	...									//检查长度
	std::string encrypted(password);	//通过copy构造函数定义并初始化
	encrypt(encrypted);
	return encrypted;
}

(2)但遇到"循环"，可能就比较困惑。如果变量只在循环内使用，那么把它定义于循环外并在每次循环迭代

时赋值给它比较好，还是该把它定义于循环内？也就是说下面两个一般性结构，哪一个比较好？

//方法A：定义于循环外
Widget w;
for(int i = 0; i < n; ++i){
	w = 取决于i的某个值;
	...
}

//方法B：定义于循环内
for(int i = 0; i < n; ++i){
	Widget w(取决于i的某个值);
	...
}

在Widget函数内部，以上两种写法的成本如下：

做法A：1个构造函数 + 1个析构函数 + n个赋值操作；

做法B：n个构造函数 + n个析构函数；

*如果classes的一个赋值成本低于一组构造+析构成本，做法A大体而言比较高效，尤其当n值很大的时候。

否则做法B或许较好。此外做法A造成名称w的作用域(覆盖整个循环)比做法B更大，有时那对程序的可理解

性和易维护性造成冲突。因此除非(1)你知道赋值成本比"构造+析构"成本低，(2)你正在处理代码中效率

高度敏感的部分，否则你应该使用做法B。
