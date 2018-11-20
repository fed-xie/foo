#include <stdbool.h>
bool isPalindrome(int x)
{
    if (x < 0)
		return false;
	
	unsigned long long x64 = x;
	unsigned long long y64 = 0;
	while (x64) {
		y64 = y64 * 10 + x64 % 10;
		x64 /= 10;
	}
	return y64 == x;
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Input a number\n");
		exit(-1);
	}
	int n = atoi(argv[1]);
	printf("%d %s a palindrome number\n", n, isPalindrome(n) ? "is" : "is not");
	return 0;
}