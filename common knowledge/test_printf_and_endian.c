#include <stdio.h>

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
	char array[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	short *pshort = (short*)array;
	int *pint = (int*)array;
	long long *pint64 = (long long*)array;
	printf("0x%x, 0x%x, 0x%x, 0x%x\n", *pshort, *(pshort+2), *pint64, *(pint+2));
	return 0;
}