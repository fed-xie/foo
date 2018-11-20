/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#include <stdlib.h>

char** letterCombinations(char* digits, int* returnSize)
{
	static const char* digstrings[] = {
		"", //0
		"",     "abc", "def",
		"ghi",  "jkl", "mno",
		"pqrs", "tuv", "wxyz"
	};
	static const int digstrlens[] = {
		0,
		0, 3, 3,
		3, 3, 3,
		4, 3, 4
	};
	if (!digits || '\0' == digits[0]) {
		*returnSize = 0;
		return NULL;
	}
	
	int diglen = 0;
	int i = 0;
	int strnum = 1;
	while (digits[i]) {
		//TODO: if strnum is too big, return
		strnum *= digstrlens[digits[i] - '0'];
		++i;
	}
	diglen = i;
	
	char** ret = malloc(strnum * sizeof(char*));
	if (NULL == ret) {
		*returnSize = 0;
		return NULL;
	}
	char* strs = malloc(strnum * (diglen+1) * sizeof(char));
	if (NULL == strs) {
		free(ret);
		*returnSize = 0;
		return NULL;
	}
	for (i=0; i<strnum; ++i)
		ret[i] = &strs[i * (diglen + 1)];
	
	int replen = strnum;
	for (int d=0; d<diglen; ++d) {
		const char* chars = digstrings[digits[d] - '0'];
		int charslen = digstrlens[digits[d] - '0'];
		int ci = 0;
		i = 0;
		replen /= charslen;
		while (i<strnum) {
			for (int j=0; j<replen; ++j)
				ret[i++][d] = chars[ci];
			ci = (ci + 1) % charslen;
		}
	}
	for (i=0; i<strnum; ++i)
		ret[i][diglen] = '\0';
	*returnSize = strnum;
	return ret;
}

#include <stdio.h>
int main(int argc, char **argv) {
	if (2 != argc) {
		fprintf(stderr, "Input numbers (2-9) (not too big)\n");
		exit(-1);
	}
    int i, size = 0;
    char ** letters = letterCombinations(argv[1], &size);
    for (i = 0; i < size; i++) {
        printf("%s\n", letters[i]);
    }
	if (letters) {
		if (letters[0])
			free(letters[0]);
		free(letters);
	}
    return 0;
}