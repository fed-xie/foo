#include <string.h>
#include <stdlib.h>
#include <stdio.h>
char* convert(char* s, int numRows)
{
	if (numRows <= 1)
		return s;
	int slen = strlen(s);
    int period = (numRows << 1) - 2;
	int trunc = slen - (slen % period);
	char* ret = malloc((slen+1) * sizeof(char));
	if (NULL == ret)
		return NULL;
	
	char* p = ret;
	int i = 0;
	while (i < slen) {
		*(p++) = s[i];
		i += period;
	}
	for (int row=1; row<numRows-1; ++row) {
		i = row;
		int incr = period - (i << 1);
		while (i < trunc) {
			*(p++) = s[i];
			*(p++) = s[i+incr];
			i += period;
		}
		if (i < slen)
			*(p++) = s[i];
		if (i+incr < slen)
			*(p++) = s[i+incr];
	}
	i = numRows - 1;
	while (i < slen) {
		*(p++) = s[i];
		i += period;
	}
	
	ret[slen] = '\0';
	return ret;
}

#include <stdio.h>
int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: ./test string\n");
        exit(-1);
    }
    printf("%s\n%s\n", argv[1], convert(argv[1], atoi(argv[2])));
    return 0;
}