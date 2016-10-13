#include <iostream>
using namespace std;

int main(void)
{
    char str1[] = "abc";
    char str2[] = "abc";
    const char str3[] = "abc";
    const char str4[] = "abc";
    char *str5 = "abc";	//warning
    char *str6 = "abc";	//warning
    const char *str7 = "abc";
    const char *str8 = "abc";

    cout << (str1 == str2) << endl;	//print 0
    cout << (str3 == str4) << endl;	//print 0
    cout << (str5 == str6) << endl;	//print 1
    cout << (str7 == str8) << endl;	//print 1
    return 0;
}