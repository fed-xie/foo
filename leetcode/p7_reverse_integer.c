#include <limits.h>
int reverse(int x)
{
	int ret = 0;
	while (x) {
		//INT32_MAX =  2147483647
		//INT32_MIN = -2147483648
		//Actually, in should be (INT_MAX - x%10)/10<ret, however,
		//8463847412 is bigger than INT_MAX, not exist in int32_t, so it's safe
		if (ret < INT_MIN/10 || INT_MAX/10 < ret)
			return 0;
		ret = ret * 10 + x % 10;
		x /= 10;
	}
	return ret;
}

#include <stdio.h>
#include <stdlib.h>

#define TEST(n, e) printf("%12d  =>  %-12d  %s!\n", n, reverse(n), e == reverse(n)?"passed":"failed")

int main(int argc, char** argv)
{
	//basic cases
    TEST(  123,  321);
    TEST( -123, -321);
    TEST( -100,   -1);
    TEST( 1002, 2001);
    //big integer
    TEST( 1463847412,  2147483641);
    TEST(-2147447412, -2147447412);
    TEST( 2147447412,  2147447412);
    //overflow
    TEST( 1000000003, 0);
    TEST( 2147483647, 0);
    TEST(-2147483648, 0);
	//customized cases
	if (argc != 2) {
		printf("Please input a number\n");
		exit(-1);
	}
	int x = atoi(argv[1]);
	printf("%12d  =>  %-12d  %s!\n", x, reverse(x), reverse(reverse(x))==x ? "passed":"failed");
	return 0;
}