/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

//vector to p22
typedef int* quadruplet;
typedef quadruplet data_t;
struct vector_t {
	int size;
	int used;
	data_t* data;
};

static void init_vector(struct vector_t* v, size_t size)
{
	if (0 == size)
		size = 4;
	v->size = size
	v->used = 0;
	v->data = malloc(size * sizeof(data_t));
	if (NULL == v->data)
		exit(-1);
}

static struct vector_t* realloc_vector(struct vector_t* v, size_t next)
{
	if (0 == next) {
		if (v->data)
			free(v->data);
		v->data = NULL;
		v->size = 0;
		v->used = 0;
		return v;
	}
	data_t* new_v = malloc(next * sizeof(data_t));
	int count = v->used < next ? v->used : next;
	for (int i=0; i<count; ++i)
		new_v[i] = v->data[i];
	if (v->data)
		free(v->data);
	v->used = count;
	v->size = next;
	v->data = new_v;
	return v;
}

static void push_back_quad(struct vector_t* vec, data_t value)
{
	if (vec->used >= vec->size)
		vec = realloc_vector(vec, vec->size ? vec->size << 1 : 4);
	vec->data[vec->used++] = value;
}

static int numcmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

static void two_sum(
	int* nums,
	int l, int r,
	int na, int nb, int target,
	struct vector_t* vec)
{
	long long tarsum = target - na - nb;
	while (l<r) {
		if (nums[l] == tarsum - nums[r]) {
			int* quad = malloc(4 * sizeof(int));
			quad[0] = na;
			quad[1] = nb;
			quad[2] = nums[l];
			quad[3] = nums[r];
			push_back_quad(vec, quad);
			do {++l;} while (l<r && nums[l] == nums[l-1]);
			if (l<r)
				do {--r;} while (l<r && nums[r] == nums[r+1]);
		}else if(nums[l] < tarsum - nums[r]) {
			do {++l;} while (l<r && nums[l] == nums[l-1]);
		}else {
			do {--r;} while (l<r && nums[r] == nums[r+1]);
		}
	}
}

int** fourSum(int* nums, int numsSize, int target, int* returnSize)
{
	if (NULL == nums || numsSize <= 3) {
		*returnSize = 0;
		return NULL;
	}
	struct vector_t res;
	init_vector(&res, 8);
	qsort(nums, numsSize, sizeof(int), numcmp);
	long long sum;
	for (int a=0; a<numsSize-3; ++a) {
		if (a > 0 && nums[a] == nums[a-1])
			continue;
		sum = nums[a] + nums[a+1] + nums[a+2] + nums[a+3];
		if (sum > target)
			break;
		sum = nums[a] + nums[numsSize-3] + nums[numsSize-2] + nums[numsSize-1];
		if (sum < target)
			continue;
		for (int b=a+1; b<numsSize-2; ++b) {
			if (b > a+1 && nums[b] == nums[b-1])
				continue;
			sum = nums[a] + nums[b] + nums[b+1] + nums[b+2];
			if (sum > target)
				break;
			sum = nums[a] + nums[b] + nums[numsSize-2] + nums[numsSize-1];
			if (sum < target)
				continue;
			two_sum(nums, b+1, numsSize-1, nums[a], nums[b], target, &res);
		}
	}
	realloc_vector(&res, res.used);
	*returnSize = res.used;
	return res.data;
}