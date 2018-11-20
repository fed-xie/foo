#include <stdlib.h>
#include <stdbool.h>

typedef int data_t;
struct stack_t {
	size_t size;
	size_t end; //index next to the last data
	data_t* data;
};

static void init_stack(struct stack_t* s, size_t size)
{
	if (0 == size)
		size = 4;
	s->size = size;
	s->end  = 0;
	s->data = malloc(size * sizeof(data_t));
	if (NULL == s->data)
		exit(-1);
}

static struct stack_t* realloc_stack(struct stack_t* s, size_t next)
{
	if (0 == next) {
		if (s->data)
			free(s->data);
		s->data = NULL;
		s->size = 0;
		s->end = 0;
		return s;
	}
	//may the next * sizeof(data_t) > SIZE_MAX ?
	data_t* new_v = malloc(next * sizeof(data_t));
	int count = s->end < next ? s->end : next;
	for (int i=0; i<count; ++i)
		new_v[i] = s->data[i];
	if (s->data)
		free(s->data);
	s->end = count;
	s->size = next;
	s->data = new_v;
	return s;
}

static void push_stack(struct stack_t* s, data_t value)
{
	if (s->end >= s->size)
		s = realloc_stack(s, s->size ? s->size << 1 : 4);
	s->data[s->end++] = value;
}

static inline data_t* pop_stack(struct stack_t* s)
{
	return s->end ? &s->data[--s->end] : NULL;
}

static inline void clear_stack(struct stack_t* s)
{
	s->end = 0;
}

static inline bool is_empty_stack(struct stack_t* s)
{
	return 0 == s->end;
}

static inline data_t* get_top(struct stack_t* s)
{
	return s->end ? &s->data[s->end - 1] : NULL;
}

#include <string.h>
int longestValidParentheses(char* s)
{
	if (!s)
		return 0;
	
	struct stack_t stack;
	init_stack(&stack, strlen(s));
	
	int wait = 0, len = 0, max = 0;
	while (*s) {
		if ('(' == *s) {
			push_stack(&stack, len);
			len = 0;
			++wait;
		}else { //')' == *s
			if (0 == wait) {
				len = 0;
				clear_stack(&stack);
			}else {
				--wait;
				len += 2;
				if (!is_empty_stack(&stack)) {
					len += *pop_stack(&stack);
				}
				if (len > max)
					max = len;
			}
		}
		++s;
	}
	realloc_stack(&stack, 0);
	return max;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	char* s = "(((()())()))(()";
	printf("s = %s, expect 12, receive %d\n", s, longestValidParentheses(s));
	s = "(())(()(())(()";
	printf("s = %s, expect 6, receive %d\n", s, longestValidParentheses(s));
	s = "(()";
	printf("s = %s, expect 2, receive %d\n", s, longestValidParentheses(s));
	s = "())())";
	printf("s = %s, expect 2, receive %d\n", s, longestValidParentheses(s));
	s = "()(())";
	printf("s = %s, expect 6, receive %d\n", s, longestValidParentheses(s));
	s = "";
	printf("s = %s, expect 0, receive %d\n", s, longestValidParentheses(s));
	
	return 0;
}