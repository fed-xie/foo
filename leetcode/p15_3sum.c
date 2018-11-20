/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */

#include <stdlib.h>

//c type vector in p18
struct trivector {
	int size;
	int num_used;
	int** triplets;
};

static void init_vector(struct trivector* v)
{
	v->triplets = malloc(sizeof(int*)*8);
	v->size = 8;
	v->num_used = 0;
}

static void realloc_vector(struct trivector* v, int next)
{
	if (0 == next) {
		if (v->triplets) {
			free(v->triplets);
			v->triplets = NULL;
		}
		return;
	}
	int** new_v = malloc(next * sizeof(int*));
	int count = v->num_used < next ? v->num_used : next;
	for (int i=0; i<count; ++i)
		new_v[i] = v->triplets[i];
	if (v->triplets)
		free(v->triplets);
	v->num_used = count;
	v->size = next;
	v->triplets = new_v;
}

static struct trivector* add_triplet(struct trivector* v, int a, int b, int c)
{
	int* triplet = malloc(3 * sizeof(int));
	if (NULL == triplet)
		return NULL;
	triplet[0] = a;
	triplet[1] = b;
	triplet[2] = c;
	if (v->num_used >= v->size)
		realloc_vector(v, v->size << 1);
	v->triplets[v->num_used++] = triplet;
	return v;
}

static int numcmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int** threeSum(int* nums, int numsSize, int* returnSize)
{
	if (numsSize < 3)
		return NULL;
	qsort(nums, numsSize, sizeof(int), numcmp);
	if (nums[0] > 0 || nums[0] + nums[1] > 0)
		return NULL;
	struct trivector res;
	init_vector(&res);
	for (int i=numsSize-1; i >= 2; --i) {
		int ni = nums[i];
		if (ni<0)
			break;
		if (i<numsSize-1 && ni == nums[i+1]) //swap the order will be faster and safe, but may not pass
			continue;
		int l = 0, r = i - 1;
		ni = -ni;
		while (l < r) {
			if (nums[l] + nums[r] == ni) {
				add_triplet(&res, nums[l], nums[r], -ni);
				do {
					++l;
				}while (nums[l] == nums[l-1] && l < r);
				do {
					--r;
				}while (nums[r] == nums[r+1] && l < r);
			}
			while (nums[l] + nums[r] < ni) {
				do {
					++l;
				}while (nums[l] == nums[l-1] && l < r);
			}
			while (nums[l] + nums[r] > ni) {
				do {
					--r;
				}while (nums[r] == nums[r+1] && l < r);
			}
		}
	}
	realloc_vector(&res, res.num_used);
	*returnSize = res.num_used;
	return res.triplets;
}

#include <stdio.h>
int main(void)
{
    int i, count;
    //int nums[] = { -1, 0, 1, 2, -1, -4 , 0, 0};
    //int nums[] = { 0, 0, 0 };
    //int nums[] = { -1, 0, 1, 0 };
	//int nums[] = {-1,-2,-3,4,1,3,0,3,-2,1,-2,2,-1,1,-5,4,-3};
	int nums[] = {-4,-2,-2,-2,0,1,2,2,2,3,3,4,4,6,6};
    //int nums[] = {-4,-4,-2,-2,-2,-2,0,0,0,2,2,2,4,8};
    int **triplets = threeSum(nums, sizeof(nums) / sizeof(*nums), &count);
    for (i = 0; i < count; i++)
        printf("%d %d %d\n", triplets[i][0], triplets[i][1], triplets[i][2]);

    return 0;
}