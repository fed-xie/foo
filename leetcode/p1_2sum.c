//Problem 1. Two Sum
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

struct sortobj {
	int val;
	int index;
};

static int cmpval(const void* a, const void* b)
{
	return ((struct sortobj*)a)->val - ((struct sortobj*)b)->val;
}

int* twoSum(int* nums, int numsSize, int target)
{
    struct sortobj* objs = malloc(numsSize * sizeof(struct sortobj));
	if (NULL == objs)
		return NULL;
	int* ret = malloc(2 * sizeof(int));
	if (NULL == ret) {
		free(objs);
		return NULL;
	}
	
	for (int i=0; i<numsSize; ++i) {
		objs[i].val = nums[i];
		objs[i].index = i;
	}
	qsort(objs, numsSize, sizeof(struct sortobj), cmpval);
	
	int i = 0, j = numsSize-1;
	while (i<j) {
		int diff = target - objs[i].val;
		if (diff > objs[j].val) {
			while (++i < j && objs[i].val == objs[i - 1].val) {}
		}else if (diff < objs[j].val) {
			while (--j > i && objs[j].val == objs[j + 1].val) {}
		}else {
			ret[0] = objs[i].index;
			ret[1] = objs[j].index;
			free(objs);
			return ret;
		}
	}
	
	free(objs);
	return NULL;
}

#include <stdio.h>
int main(void)
{
    //int nums[] = {-1, -2, -3, -4, -5};
    //int target = -8;
    int nums[] = {0,4,3,0};
    int target = 0;
    //int nums[] = { 3, 2, 3 };
    int count = sizeof(nums) / sizeof(*nums);
    //int target = 6;
    int *indexes = twoSum(nums, count, target);
    if (indexes != NULL) {
        printf("%d %d\n", indexes[0], indexes[1]);
    } else {
        printf("Not found\n");
    }

    return 0;
}