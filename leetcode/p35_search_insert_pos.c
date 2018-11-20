int searchInsert(int* nums, int numsSize, int target)
{
	if (!nums)
		return 0;
	int l = 0, h = numsSize - 1, m;
	while (l<h) {
		m = (l+h)/2;
		if (nums[m] < target)
			l = m + 1;
		else
			h = m - 1;
	}
	return nums[l] < target ? l+1 : l;
}