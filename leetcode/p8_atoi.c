#include <limits.h>
#include <ctype.h>
int myAtoi(char* str)
{
    if (NULL == str)
		return 0;
	while (' ' == *str)
		++str;
	
	int negative = 0;
	if ('\0' == *str) {
		return 0;
	}else if ('-' == *str) {
		negative = 1;
		++str;
	}else if ('+' == *str) {
		++str;
	}
	
	//INT32_MAX =  2147483647, 10 digits in decimalism
	//INT32_MIN = -2147483648
	if (isdigit(*str)) {
		while ('0' == *str)
			++str;
		
		int ret = 0;
		//calc 9 digits
		for (int digit=0; isdigit(*str) && digit<9; ++digit, ++str)
			ret = ret * 10 + (*str - '0');
		
		if (isdigit(*str)) {
			int num = *str - '0';
			if (negative) {
				ret *= -1;
				if (ret < (INT_MIN + num)/10)
					return INT_MIN;
				else
					ret = ret * 10 - num;
			}else {
				if ((INT_MAX - num)/10 < ret)
					return INT_MAX;
				else
					ret = ret * 10 + num;
			}
			++str;
		}else {
			if (negative)
				ret *= -1;
			return ret;
		}
		
		if (isdigit(*str))
			return negative ? INT_MIN : INT_MAX;
		return ret;
	}
	return 0;
}

#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./atoi 123\n");
        exit(-1);
    }

    int n = myAtoi(argv[1]);
    printf("n = %d %x\n", n, n);

    return 0;
}