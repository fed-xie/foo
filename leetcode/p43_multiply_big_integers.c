#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* multiply(char* num1, char* num2)
{
	assert(num1 && num2 && *num1 && *num2);
	int l1 = strlen(num1), l2 = strlen(num2);
	//int negative;
	int carry = 0;
	int res_size = sizeof(char)*(l1+l2);
	char* result = malloc(res_size);
	memset(result, '0', res_size);
	for (int i1 = l1-1; i1>=0; --i1) {
		int n1 = nums[i1] - '0';
		for (int i2 = l2-1; i2>=0; --i2) {
			int n2 = nums[i2] - '0';
			int product = n1 * n2;
			
		}
	}
	
	return result;
}

#include <stdio.h>
#include <limits.h>
int main(int argc, char** argv)
{
	printf("UINA_MAX = %u\n", UINT_MAX);
	if (argc != 3) {
		printf("Input two number\n");
		return -1;
	}
	char* result = multiply(argv[1], argv[2]);
	printf("%s\n", result);
	if (result)
		free(result);
	return 0;
}