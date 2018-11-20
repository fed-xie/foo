int search(int* nums, int numsSize, int target)
{
	if (0 == numsSize)
		return -1;
	
	int l = 0, h = numsSize - 1, m;
	while (l<h) {
		m = (l+h)/2;
		if (target == nums[m])
			return m;
		
		if (nums[l] < nums[h]) { //ascending
			if (nums[m] < target)
				l = m + 1;
			else
				h = m - 1;
		}else if (nums[l] <= nums[m]) { //m before the falldown point
			if (nums[m] < target || target <= nums[h])
				l = m + 1;
			else
				h = m - 1;
		}else { //m after the falldown point
			if (nums[l] <= target || target < nums[m])
				h = m - 1;
			else
				l = m + 1;
		}
	}
	
	return target == nums[h] ? h : -1;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int a[] = {4,5,6,7,9,1,2,3};
	int len = sizeof(a)/sizeof(*a);
	printf("search 4, expect 0, return %d\n", search(a, len, 4));
	printf("search 5, expect 1, return %d\n", search(a, len, 5));
	printf("search 6, expect 2, return %d\n", search(a, len, 6));
	printf("search 9, expect 4, return %d\n", search(a, len, 9));
	printf("search 0, expect -1, return %d\n", search(a, len, 0));
	printf("search 8, expect -1, return %d\n", search(a, len, 8));
	printf("search 1, expect 5, return %d\n", search(a, len, 1));
	printf("search 2, expect 6, return %d\n", search(a, len, 2));
	printf("search 3, expect 7, return %d\n", search(a, len, 3));
	printf("\n");
	int b[] = {4, 3};
	printf("search 3, expect 1, return %d\n", search(b, 2, 3));
	printf("search 4, expect 0, return %d\n", search(b, 2, 4));
	printf("search 2, expect -1, return %d\n", search(b, 2, 2));
	printf("search 5, expect -1, return %d\n", search(b, 2, 5));
	printf("\n");
	int c[] = {6,7,8,9,11,0,1,2,3,4,5};
	len = sizeof(c)/sizeof(*c);
	printf("search 11, expect 4, return %d\n", search(c, len, 11));
	printf("search 10, expect -1, return %d\n", search(c, len, 10));
	printf("\n");
	
	return 0;
}

/*
45679123
l  m   h	9
    lm h
*/