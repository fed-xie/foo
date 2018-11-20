/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

typedef char *data_t;
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

static int ssize;
#include <string.h>
static void fill(struct vector_t* v, char* str, int left, int wait)
{
	if (0 == left && 0 == wait) {
		char* s = malloc(ssize * sizeof(char));
		strcpy(s, str-ssize+1);
		push_back_vector(v, s);
		return;
	}
	if (wait) {
		*str = ')';
		fill(v, str+1, left, wait-1);
	}
	if (left) {
		*str = '(';
		fill(v, str+1, left-1, wait+1);
	}
}

char** generateParenthesis(int n, int* returnSize)
{
	if (0 == n) {
		*returnSize = 0;
		return NULL;
	}
	
	struct vector_t vec;
	init_vector(&vec, 4);
	ssize = 2 * n + 1;
	char* stack = malloc(ssize * sizeof(char));
	stack[ssize-1] = '\0';
	
	fill(&vec, stack, n, 0);
	
	*returnSize = vec.used;
	realloc_vector(&vec, vec.used);
	return vec.data;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int size;
	char** strs = generateParenthesis(5, &size);
	printf("size = %d\n", size);
	for (int i=0; i<size; ++i) {
		printf("%s ", strs[i]);
		free(strs[i]);
	}
	if (strs)
		free(strs);
	printf("\n");
	return 0;
	
}