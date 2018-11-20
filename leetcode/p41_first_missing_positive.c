int firstMissingPositive(int* nums, int numsSize)
{
	if (0 == numsSize)
		return 1;
	for (int i=0; i<numsSize; ++i) {
		int iv = nums[i];
		int tv = nums[iv];
		while (iv != i && iv > 0 && iv < numsSize && tv != iv) {
			nums[iv] = iv;
			iv = nums[i] = tv;
			tv = nums[iv];
		}
	}
	for (int i=1; i<numsSize; ++i) {
		if (nums[i] != i)
			return i;
	}
	return nums[0] == numsSize ? numsSize + 1 : numsSize;
}

#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int *nums = malloc(sizeof(int)*(argc-1));
	for (int i=1; i<argc; ++i)
		nums[i-1] = atoi(argv[i]);
	printf("output: %d\n", firstMissingPositive(nums, argc-1));
	return 0;
}