#include <stdlib.h>
#include <stdbool.h>

//stack to p32
typedef char data_t;
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

static inline bool is_empty_stack(struct stack_t* s)
{
	return 0 == s->end;
}

static inline data_t* get_top(struct stack_t* s)
{
	return s->end ? &s->data[s->end - 1] : NULL;
}

#include <string.h>

bool isValid_use_stack(char* s)
{
	//reversive solution may be too wasty
	size_t slen = strlen(s);
	struct stack_t stack;
	init_stack(&stack, slen);
	while (*s) {
		if ('(' == *s) {
			push_stack(&stack, ')');
		}else if ('[' == *s) {
			push_stack(&stack, ']');
		}else if ('{' == *s) {
			push_stack(&stack, '}');
		}else if (is_empty_stack(&stack) || *pop_stack(&stack) != *s) {
			realloc_stack(&stack, 0);
			return false;
		}
		++s;
	}
	if (!is_empty_stack(&stack)) {
		realloc_stack(&stack, 0);
		return false;
	}
	realloc_stack(&stack, 0);
	return true;
}

#include <stdbool.h>
#include <stdio.h>

bool isValid_recursive(char** s_ref, char wait)
{
	char c = **s_ref;
	++(*s_ref);
	if (c == wait)
		return true;
	if ('0' == c)
		return false;
	if ('(' == c) {
		if (!isValid_recursive(s_ref, ')'))
			return false;
		return isValid_recursive(s_ref, wait);
	}else if ('[' == c) {
		if (!isValid_recursive(s_ref, ']'))
			return false;
		return isValid_recursive(s_ref, wait);
	}else if ('{' == c) {
		if (!isValid_recursive(s_ref, '}'))
			return false;
		return isValid_recursive(s_ref, wait);
	}
	return false;
}

bool isValid(char* s)
{
	if (NULL == s)
		return true;
	//return isValid_use_stack(s);
	return isValid_recursive(&s, '\0');
}

#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./test xxxx");
        exit(-1);
    }
    printf("%s\n", isValid(argv[1]) ? "true" : "false");
    return 0;
}