#include <string.h>
#include <stdint.h>

static uint64_t hash(char* str, int* len)
{
	register uint64_t h = 0;
	char* s = str;
	while (*s)
		h = (h << 5) - h + *s++;
	if (len)
		*len = s - str;
	return h;
}

int strStr(char* haystack, char* needle)
{
	if (NULL == needle || '\0' == needle)
		return 0;
	if (NULL == haystack)
		return -1;
	
	int nlen = 0;
	uint64_t nhash = hash(needle, &nlen);
	uint64_t hhash = 0;
	uint64_t cpow = 1;
	for (int i=0; i<nlen; ++i) {
		if ('\0' == haystack[i])
			return -1;
		hhash = (hhash << 5) - hhash + haystack[i];
		cpow = (cpow << 5) - cpow;
	}
	if (hhash == nhash && 0 == strncmp(needle, haystack, nlen))
		return 0;
	
	cpow /= 31;
	int i = 0;
	while ('\0' != haystack[i+nlen]) {
		hhash = hhash - haystack[i] * cpow;
		hhash = (hhash << 5) - hhash + haystack[i+nlen];
		if (hhash == nhash && 0 == strncmp(needle, &haystack[i+1], nlen))
			return i+1;
		++i;
	}
	return -1;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	if (3 != argc) {
		printf("input string and pattern\n");
		return -1;
	}
	printf("str=\"%s\"\npattern=\"%s\"\nans=%d\n", argv[1], argv[2], strStr(argv[1], argv[2]));
	return 0;
}