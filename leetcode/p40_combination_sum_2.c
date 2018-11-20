/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *columnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

typedef struct array_t {
	int size;
	int* data;
}array_t;
typedef array_t data_t;
struct vector_t {
	size_t size;
	size_t used;
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

static int process(int* cands, int cand_num, int target, int used_num, struct vector_t* combs)
{
	if (0 == target) {
		struct array_t comb;
		comb.size = used_num;
		comb.data = malloc(sizeof(int) * comb.size);
		push_back_vector(combs, comb);
		return 1;
	}
	if (0 == cand_num || *cands > target)
		return 0;
	
	int candidate = *cands;
	if (candidate == target) {
		struct array_t comb;
		comb.size = used_num + 1;
		comb.data = malloc(sizeof(int) * comb.size);
		//memset(comb.data, 0, sizeof(int) * comb.size);
		comb.data[used_num] = candidate;
		push_back_vector(combs, comb);
		return 1;
	}
	
	int total = 0;
	int occupied = combs->used;
	int rep = 1;
	while (rep < cand_num && candidate == cands[rep])
		++rep;
	for (int i=1; i<=rep; ++i) {
		int num = process(cands+rep, cand_num-rep, target-candidate*i, used_num+i, combs);
		for (int comb=0; comb<num; ++comb) {
			for (int j=0; j<i; ++j)
				combs->data[occupied+total+comb].data[used_num+j] = candidate;
		}
		total += num;
	}
	total += process(cands+rep, cand_num-rep, target, used_num, combs);
	return total;
}

static int numcmp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

#include <assert.h>
int** combinationSum2(int* candidates, int candidatesSize, int target, int** columnSizes, int* returnSize)
{
	assert(columnSizes && returnSize && target);
	if (NULL == candidates) {
		*columnSizes = NULL;
		*returnSize = 0;
		return NULL;
	}
	
	struct vector_t combs;
	init_vector(&combs, 4);
	
	qsort(candidates, candidatesSize, sizeof(int), numcmp);
	int total = process(candidates, candidatesSize, target, 0, &combs);
	if (0 == total) {
		realloc_vector(&combs, 0);
		*columnSizes = NULL;
		*returnSize = 0;
		return NULL;
	}
	
	*columnSizes = malloc(sizeof(int) * total);
	int** res = malloc(sizeof(int*) * total);
	for (int i=0; i<total; ++i) {
		(*columnSizes)[i] = combs.data[i].size;
		res[i] = combs.data[i].data;
	}
	*returnSize = total;
	
	realloc_vector(&combs, 0);
	return res;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("Input target and candidates\n");
		return -1;
	}
	
	int target = atoi(argv[1]);
	int candSize = argc - 2;
	int* candidates = malloc(sizeof(int) * (argc - 2));
	for (int i=0; i<candSize; ++i)
		candidates[i] = atoi(argv[i+2]);
	
	int* columnSizes;
	int resSize;
	int** res = combinationSum2(candidates, candSize, target, &columnSizes, &resSize);
	if (res) {
		for (int i=0; i<resSize; ++i) {
			printf("%d. ", i);
			for (int j=0; j<columnSizes[i]; ++j)
				printf("%d ", res[i][j]);
			printf("\n");
			free(res[i]);
		}
		free(res);
		free(columnSizes);
	}else {
		printf("fail\n");
	}
	return 0;
}