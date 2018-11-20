#include <stdlib.h>

static int numcmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int threeSumClosest(int* nums, int numsSize, int target)
{
	long long res = nums[0] + nums[1] + nums[2];
	if (numsSize <= 3)
		return res;
	long long sum, diff = llabs(target - res);
	qsort(nums, numsSize, sizeof(int), numcmp);
	for (int i=0; i<numsSize-2; ++i) {
		if (i > 0 && nums[i] == nums[i-1])
			continue;
		int l = i + 1, r = numsSize - 1;
		while (l<r) {
			sum = nums[i] + nums[l] + nums[r];
			int diffilr = llabs(target - sum);
			if (diffilr < diff) {
				res = sum;
				if (sum == target)
					return sum;
				diff = diffilr;
			}
			sum < target ? ++l : --r;
		}
	}
	return res;
}