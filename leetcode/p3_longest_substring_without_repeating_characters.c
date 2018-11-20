#include <string.h>

int lengthOfLongestSubstring(char* s)
{
	int pos[1<<8];
	int res = 0;
	int beg = 0, end = 0;
	
	memset(pos, -1, sizeof(pos));
	char c = s[end];
	while ('\0' != c) {
		int op = pos[c];
		if (op >= beg)
			beg = ++op;
		pos[c] = end++;
		int wsize = end - beg;
		if (wsize > res)
			res = wsize;
		c = s[end];
	}
	return res;
}

#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	//KMP next generator
	if (1 >= argc) {
		printf("Input test string\n");
		exit(-1);
	}
	
	const char* str = argv[1];
	printf("%d\n", lengthOfLongestSubstring(argv[1]));
	return 0;
}

//ABAABCAC