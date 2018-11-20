int jump(int* nums, int numsSize)
{
	if (numsSize <= 1)
		return 0;
	int step = 0, cur_max = 0, next_max = 0;
	int i = 0, target = numsSize - 1;
	while (i <= cur_max) {
		++step;
		while (i <= cur_max) { //search the boundary
			if (i + nums[i] > next_max)
				next_max = i + nums[i];
			if (next_max >= target)
				return step;
			++i;
		}
		cur_max = next_max;
	}
	return 0;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int nums[] = {2,3,1,1,4};
	int n = sizeof(nums)/sizeof(*nums);
	printf("%d\n", jump(nums, n));
	return 0;
}