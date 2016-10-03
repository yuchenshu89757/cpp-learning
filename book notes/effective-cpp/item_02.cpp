1. 以常量替换宏

（1）定义语言常量

#define ASPECT_RATIO 1.653

-> const double AspectRatio = 1.653;

（2）定义常量指针

const char* const authorName = "Scott Meyers";

更合宜版本：const std::string autherName("Scott Meyers");

(3)定义class专属常量，为将常量作用域限制于class内，必须让它成为class的一个成员；

而为确保此常量至多只有一份实体，必须让它成为一个static成员

class GamePlayer{

private:

static const int NumTurns; 	//使用常量声明式
		
int scores[NumTurns];		//使用该常量
		
}

const int GamePlayer::NumTurns = 5;	//NumTurns的定义

如果编译器不支持上述做法，可以采用the enum hack方法

class GamePlayer{

private:

enum{ NumTurns = 5 };

int scores[NumTurns];

}

2. 不要用#define实现宏

例如，有如下宏实现两个数中的较大者作为f的实参

#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b)))

如果在程序中这样使用

int a = 5, b = 0;

CALL_WITH_MAX(++a, b);	//a被累加2次

CALL_WITH_MAX(++a, b + 10)； //a被累加1次

在这里，调用f之前，a递增的次数竟然取决于“它被拿来和谁比较”

解决方法是用template inline 函数代替

template <typename T>
inline void callWithMax(const T& a, const T& b)
{
     f(a > b ? a : b);
}

