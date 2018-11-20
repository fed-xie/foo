#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
int maxArea(int* height, int heightSize)
{
	int l = 0, r = heightSize - 1;
	int maxlv = min(height[l], height[r]);
	int max, area;
	max = area = maxlv * r;
	while (l < r) {
		if (height[l] < height[r]) {
			do {
				++l;
			}while (height[l] <= maxlv);
		}else {
			do {
				--r;
			}while (height[r] <= maxlv);
		}
		
		maxlv = min(height[l], height[r]);
		area = maxlv * (r - l);
		if (max < area)
			max = area;
	}
	return max;
}