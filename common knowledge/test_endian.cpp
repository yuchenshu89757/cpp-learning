#include <iostream>
using namespace std;

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 0
int test_byte_order()
{
	short word = 0x0001;
	char *byte = (char*)&word;
	return byte[0] ? LITTLE_ENDIAN : BIG_ENDIAN;
}

int main(void)
{
	cout << test_byte_order() << endl;
}