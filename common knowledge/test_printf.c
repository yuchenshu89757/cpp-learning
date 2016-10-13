#include <stdio.h>

int main()
{
	char array[12] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	short *pshort = (short*)array;
	int *pint = (int*)array;
	long long *pint64 = (long long*)array;
	printf("0x%x, 0x%x, 0x%x, 0x%x\n", *pshort, *(pshort+2), *pint64, *(pint+2));
	return 0;
}