Avoid returning "handles" to object internals

避免返回handles指向对象内部成分

1. 起因：Reference、指针和迭代器都是所谓的handles(号码牌，用来取得某个对象)，而返回一个

"代表对象内部数据"的handle，随之而来的便是"降低对象封装性"的风险。

2. 举例

(1) 假设你的程序涉及矩形。每个矩形由其左上角和右下角表示。为了让一个Rectangle对象尽可能小，

你可能会决定不把定义矩形的这些点存放在Rectangle对象内，而是放在一个辅助的struct内再让

Rectangle去指它：

class Point{	//这个class用来表述点
public:
	Point(int x, int y);
	...
	void setX(int newVal);
	void setY(int newVal);
	...
};

struct RectData{	//这些点数据用来表现一个矩形
	Point ulhc;		//upper left-hand corner
	Point lrhc;		//lower right-hand corner
};

class Rectangle{
public:
	Point& upperLeft()const{return pData->ulhc;}
	Point& lowerRight()const{return pData->lrhc;}
private:
	std::tr1::shared_ptr<RectData> pData;
};

其中，upperLeft函数和lowerRight函数用于计算Rectangle的范围，这样设计可通过编译，但却是

错误的。实际上它是自我矛盾的。一方面upperLeft和lowerRight被声明为const成员函数，因为它

们的目的只是为了提供客户一个得知Rectangle相关坐标点的方法，而不是让客户修改Rectangle，

另一方面两个函数却都返回references指向private内部数据，调用者于是通过这些references更改

内部数据，例如：

Point coord1(0,0);

Point coord2(10,10);

const Rectangle rec(coord1, coord2);//rec是个const矩形，从(0,0)到(10,10)

rec.upperLeft().setX(5);	//现在却变成了(5,0)到(10, 10)

这里，upperLeft的调用者能够使用被返回的reference来更改成员，但rec其实是不可变的。为了

解决这个问题，我们做如下更改：

class Rectangle{
public:
	...
	const Point& upperLeft()const{return pData->ulhc;}
	const Point& lowerRight()const{return pData->lrhc;}
	...
};

有了这样的改变，客户可以读取矩形的Points, 但不能修改它们。至于封装问题，我们总是愿意

让客户看到Rectangle的外围Points，所以这里蓄意放松封装，更重要的是这是个有限度的放松：

这些函数只让渡读取权，涂写权仍然是被禁止的。

(2)但即使如此，upperLeft和lowerRight还是返回了"代表对象内部"的handles，有可能在其他场合

带来问题。更明确地说，它可能导致dangling handles(空悬的号码牌):这种handles所指东西(

的所属对象)不复存在，这种"不复存在的对象"最常见的来源旧式函数返回值。例如某个函数返回

GUI对象的外框，这个外框采用矩形形式：

class GUIObject{...};

const Rectangle boundingBox(const GUIObject& obj);

现在，客户有可能这么使用这个函数：

GUIObject* pgo;

...

const Point* pUpperLeft = &(boundingBox(*pgo).upperLeft());

对boundingBox的调用获得一个新的、暂时的Rectangle对象temp, 随后upperLeft作用于temp身上，

返回一个reference指向temp的一个用以标示temp的Points，于是pUpperLeft指向那个Point对象，

目前为止一切还好，但故事还未结束，因为在那个语句结束之后，boundingBox的返回值，也就是

我们所说的temp，将被销毁，而那间接导致temp内的Points析构，最终导致pUpperLeft指向一个

不再存在的对象，也就是说一旦产出pUpperLeft的那个语句结束，pUpperLeft也就变成空悬。

3. 总结：避免返回handles(包括reference,指针和迭代器)指向对象内部。遵守这个条款可增加封

装性，帮助const成员函数的行为像个const, 并将发生"虚吊号码牌"的可能性降至最低。
