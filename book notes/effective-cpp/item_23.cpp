Prefer non-member non-friend functions to member functions

宁以non-member, non-friend 替换 member函数

1.起因：封装的好处在于，它使我们能够改变事物而只影响有限客户。如果某些东西被封装，它就不再可见。

越多东西被封装，越少人可以看到它，我们就有越大的弹性去变化它，因为我们的改变仅仅直接影响看到改变

的那些人事物。

(1) 成员变量应该是private, 否则就毫无封装性；

(2) 能够访问private成员变量的函数只有class的member函数和friend函数。

如果要在一个member函数和一个non-member,non-friend函数之间作抉择，而且两者提供相同机能，那么，导致

较大封装性的是non-member non-friend函数，因为它并不增加"能够访问class内private成分"的函数数量。

*这里有两件事情值得注意：

(1)由于friend函数对class private成员的访问权限和member函数相同，因此从封装的角度看，这里选择关键

并不在member和non-member函数之间，而是在member和non-member non-friend函数之间。

(2)只因在意封装性而让函数"成为class的non-member"，并不意味着它"不可以是另一个class的member"。我们

完全可以让那些函数成为某个工具类的一个static member函数，只要它不是当前class的一部分(或成为其friend),

就不会影响当前class的private成员封装性。

2. 解决方法：让此函数成为一个non-member函数，并且位于当前类所在的同一个namespace内。

例如，一个浏览器类和它的一个non-member,non-friend函数之间。

namespace WebBrowserStuff{
	
	class WebBrowser{...};
	
	void clearBrowser(WebBrowser& wb);
	
}

(1) namespace 可以跨越多个源码文件而 class 不可以；

(2) 可以按照模块来声明头文件，每个头文件只声明一种机能，然后用同一个 namespace 组织将这些模块组

织在一起。例如：

//头文件"webbrowser.h", 这个头文件针对class WebBrowser自身及WebBrowser核心机能
namespace WebBrowserStuff{
	
	class WebBrowser{...};
	
	...
	
}

//头文件"webbrowser_bookmarks.h",与书签相关的便利函数

namespace WebBrowserStuff{
	
	...
	
}

//头文件"webbrowser_cookies.h"

namespace WebBrowserStuff{
	
	...
	
}

将所有便利函数放在多个头文件内但隶属同一个命名空间，意味客户可以轻松扩展这一组便利函数，他们

需要做的就是添加更多non-member non-friend函数到此命名空间内。
