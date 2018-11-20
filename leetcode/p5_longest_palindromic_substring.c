#include <stdlib.h>
#include <string.h>
char* longestPalindrome(char* s)
{
    int mid = 0;
	int len = 0;
	char* ret = "";
	if (NULL == s || '\0' == s)
		return ret;
	if ('\0' == s[1])
		return s;
	while ('\0' != s[mid]) {
		int i = 1;
		while (0 <= mid-i+1) {
			if (s[mid-i+1] == s[mid+i]) {
				if (len < (i<<1)) {
					len = i<<1;
					ret = &s[mid-i+1];
				}
			}else {
				break;
			}
			++i;
		}
		i = 1;
		while (0 <= mid-i) {
			if (s[mid-i] == s[mid+i]) {
				if (len < (i<<1)+1) {
					len = (i<<1)+1;
					ret = &s[mid-i];
				}
			}else {
				break;
			}
			++i;
		}
		++mid;
	}
	
	if (len) {
		s = ret;
		ret = malloc(sizeof(char)*(len+1));
		if (NULL == ret)
			return NULL;
		strncpy(ret, s, len);
		ret[len] = '\0';
	}
	return ret;
}

#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./test string\n");
        exit(-1);
    }
    printf("%s\n", longestPalindrome(argv[1]));
    return 0;
}