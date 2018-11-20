#if 0
int trap(int* height, int heightSize)
{
	int l = 0, r = heightSize - 1;
	while (l < r && height[l] <= height[l+1])
		++l;
	while (l < r && height[r-1] >= height[r])
		--r;
	
	int volume = 0;
	while (l < r) {
		int water_level;
		if (height[l] <= height[r]) {
			water_level = height[l];
			while (++l < r && height[l] < water_level)
				volume += water_level - height[l];
			while (l < r && height[l] <= height[l+1])
				++l;
		}else {
			water_level = height[r];
			while (l < --r && height[r] < water_level)
				volume += water_level - height[r];
			while (l < r && height[r-1] >= height[r])
				--r;
		}
	}
	return volume;
}
#endif

int trap(int* height, int n) {
    int level = 0, water = 0;
    while (n--) {
        int lower = *height < height[n] ? *height++ : height[n];
        if (lower > level)
			level = lower;
        water += level - lower;
    }
    return water;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int height[] = {0,1,0,2,1,0,1,3,2,1,2,1};
	int num = sizeof(height)/sizeof(*height);
	printf("output %d\n", trap(height, num));
	return 0;
}