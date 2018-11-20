/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

int* searchRange(int* nums, int numsSize, int target, int* returnSize)
{
	int *ret = malloc(sizeof(int)*2);
	if (NULL == ret) {
		*returnSize = 0;
		return NULL;
	}
	
	ret[0] = ret[1] = -1;
	*returnSize = 2;
	if (NULL == nums)
		return ret;
	
	int l = 0, h = numsSize - 1, m;
	while (l<h) {
		m = (l+h)/2;
		if (target == nums[m])
			break;
		else if (target < nums[m])
			h = m - 1;
		else
			l = m + 1;
	}
	m = (l+h)/2;
	if (target != nums[m])
		return ret;
	
	l = 0;
	ret[1] = h = m;
	while (l<h) {
		m = (l+h)/2;
		if (nums[m] == target)
			h = m;
		else
			l = m + 1;
	}
	ret[0] = h;
	
	l = ret[1];
	h = numsSize - 1;
	while (l<h) {
		m = (l+h)/2;
		if (nums[m] == target)
			l = m + 1;
		else
			h = m - 1;
	}
	ret[1] = nums[h] == target ? h : h - 1;
	
	return ret;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int a[] = {5,7,7,8,8,10};
	int n = sizeof(a)/sizeof(*a);
	int *ret, asize;
	ret = searchRange(a, n, 8, &asize);
	printf("expect [3, 4], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	ret = searchRange(a, n, 7, &asize);
	printf("expect [1, 2], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	ret = searchRange(a, n, 6, &asize);
	printf("expect [-1, -1], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	
	int b[] = {5, 5, 5, 5};
	n = sizeof(b)/sizeof(*b);
	ret = searchRange(b, n, 5, &asize);
	printf("expect [0, 3], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	ret = searchRange(b, n, 1, &asize);
	printf("expect [-1, -1], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	
	ret = searchRange(NULL, 0, 3, &asize);
	printf("expect [-1, -1], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	
	int c[] = {3};
	n = sizeof(c)/sizeof(*c);
	ret = searchRange(c, n, 3, &asize);
	printf("expect [0, 0], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	ret = searchRange(c, n, 5, &asize);
	printf("expect [-1, -1], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	
	int d[] = {1, 4};
	n = sizeof(d)/sizeof(*d);
	ret = searchRange(d, n, 4, &asize);
	printf("expect [1, 1], return [%d,%d]\n", ret[0], ret[1]);
	free(ret);
	return 0;
}