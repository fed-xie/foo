/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

typedef int* data_t;
struct vector_t {
	size_t size;
	size_t used;
	int max_depth;
	data_t* data;
};

static void init_vector(struct vector_t* v, size_t size)
{
	if (0 == size)
		size = 4;
	v->size = size;
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
	//realloc is better, but leetcode not supported
	data_t* new_v = malloc(next * sizeof(data_t));
	int count = v->used < next ? v->used : next;
	for (int i=0; i<count; ++i)
		new_v[i] = v->data[i];
	if (v->data)
		free(v->data);
	v->data = new_v;
	
	v->used = count;
	v->size = next;
	return v;
}

static void push_back_vector(struct vector_t* vec, data_t value)
{
	if (vec->used >= vec->size)
		vec = realloc_vector(vec, vec->size ? vec->size << 1 : 4);
	vec->data[vec->used++] = value;
}

static int fill_permutations(int* nums, int depth, struct vector_t* vec)
{
	if (depth == vec->max_depth) {
		int* arr = malloc(sizeof(int)*(depth+1));
		arr[depth] = *nums;
		push_back_vector(vec, arr);
		return 1;
	}
	int base = vec->used;
	int total = 0;
	int size = vec->max_depth - depth + 1;
	int val;
	for (int i=0; i<size; ++i) {
		val = nums[i];
		nums[i] = nums[0];
		int num = fill_permutations(nums+1, depth+1, vec);
		for (int j=0; j<num; ++j)
			vec->data[base+total+j][depth] = val;
		nums[i] = val;
		total += num;
	}
	return total;
}

int** permute(int* nums, int numsSize, int* returnSize)
{
	if (0 == numsSize) {
		*returnSize = 0;
		return NULL;
	}
	
	*returnSize = 1;
	for (int i=2; i<=numsSize; ++i)
		*returnSize *= i;
	
	struct vector_t res;
	init_vector(&res, *returnSize);
	
	res.max_depth = numsSize - 1;
	fill_permutations(nums, 0, &res);
	
	//realloc_vector(&res, res.size);
	return res.data;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int nums[] = {1,2,3};
	int n = sizeof(nums)/sizeof(*nums);
	int ret_size;
	int** res = permute(nums, n, &ret_size);
	for (int i=0; i<ret_size; ++i) {
		printf("%d. ", i);
		for(int j=0; j<n; ++j)
			printf("%d ", res[i][j]);
		printf("\n");
		free(res[i]);
	}
	if (res)
		free(res);
	return 0;
}