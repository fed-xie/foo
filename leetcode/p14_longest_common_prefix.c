#include <string.h>
#include <stdlib.h>

char* longestCommonPrefix(char** strs, int strsSize)
{
	if (0 == strsSize)
		return "";
	if (1 == strsSize)
		return strs[0];
	
	char *s1 = strs[0];
	char *s2 = strs[1];
	while (*s1 && *s1 == *s2) {
		++s1;
		++s2;
	}
	int comlen = s1 - strs[0];
	if (0 == comlen)
		return "";
	
	char sentinel = strs[0][comlen];
	strs[0][comlen] = '\0';
	for (int i = 2; i < strsSize; ++i) {
		s1 = strs[0];
		s2 = strs[i];
		while (*s1 && *s1 == *s2) {
			++s1;
			++s2;
		}
		if (*s1) {
			strs[0][comlen] = sentinel;
			comlen = s1 - strs[0];
			if (0 == comlen)
				return "";
			sentinel = strs[0][comlen];
			strs[0][comlen] = '\0';
		}
	}
	s2 = malloc((comlen + 1)*sizeof(char));
	if (NULL == s2)
		return NULL;
	s1 = strs[0];
	strcpy(s2, s1);
	strs[0][comlen] = sentinel;
	return s2;
}

#include <stdio.h>
int main(int argc, char **argv)
{
    printf("%s\n", longestCommonPrefix(argv + 1, argc - 1));
    return 0;
}