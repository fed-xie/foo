#include <stdbool.h>
#include <stdint.h>

static bool match(char* s, char* p)
{
	//assert('*' != s[0]);
	bool fst_match;
	while ('\0' != *p && '\0' != *s ) {
		fst_match = *s == *p || '.' == *p;
		if ('*' == p[1])
			return match(s, p+2) || fst_match && match(s+1, p);
		if (!fst_match)
			return false;
		else {
			++p;
			++s;
		}
	}
	while ('\0' != *p && '*' == *(p+1)) //skip elements like "a*b*c*"
		p += 2;
	if ('\0' != *p)
		return false;
	return '\0' == *s;
}

bool isMatch(char* s, char* p)
{
	if (NULL == s && NULL == p)
		return true;
	if (NULL == s || NULL == p)
		return false;
	
	//assert('*' != s[0]);
	return match(s, p);
}

//static bool isMatch(char* s, char* p)
//{
//    if (*p == '\0') {
//        return *s == '\0';
//    }
//    /* p's length 1 is special case */
//    if (*(p + 1) == '\0' || *(p + 1) != '*') {
//        if (*s == '\0' || ( *p != '.' && *s != *p)) {
//            return false;
//        } else {
//            return isMatch(s + 1, p + 1);
//        }
//    }
//    int len = strlen(s);
//    int i = -1;
//    while (i < len && (i < 0 || *p == '.' || *p == *(s + i))) {
//        if (isMatch(s + i + 1, p + 2)) {
//            return true;
//        }
//        i++;
//    }
//    return false;
//}

#include <stdio.h>
int main(int argc, char **argv)
{
	struct test_case {
		char* string;
		char* regular;
		bool expect;
	};
	struct test_case cases[] = {
		{"aab", "a", false},
		{"aab", "a*b", true},
		{"aab", "c*a*", false},
		{"aab", "c*a*b", true},
		{"aab", "c*a*b*", true},
		{"aab", "c*a*b*d*b*b*", true},
		{"", "", true},
		{"mississippi", "mis*is*p*.", false},
	};
	const int count = sizeof(cases)/sizeof(cases[0]);
	for (int i=0; i<count; ++i)
		printf("%s\t%s\t %d %d\n", cases[i].string, cases[i].regular,
			cases[i].expect,
			isMatch(cases[i].string, cases[i].regular));
	
    if (3 == argc)
		printf("%s\n", isMatch(argv[1], argv[2]) ? "true" : "false");
    return 0;
}