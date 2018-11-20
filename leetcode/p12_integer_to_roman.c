char* intToRoman(int num)
{
	static char* M[] = {"", "M", "MM", "MMM"};
    static char* C[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    static char* X[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    static char* I[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
	char roman[16];
	char *p = roman, *a;
	a = M[num / 1000];
	while (*a)
		*p++ = *a++;
	a = C[(num%1000)/100];
	while (*a)
		*p++ = *a++;
	a = X[(num%100)/10];
	while (*a)
		*p++ = *a++;
	a = I[num%10];
	while (*a)
		*p++ = *a++;
	*p = '\0';
	if (p - roman) {
		a = malloc((p - roman + 1) * sizeof(char));
		p = roman;
		char *s = a;
		while (*s++ = *p++)
			;
		return a;
	}
	return "";
}

//MMM CM XC IX 3999
//MMM DCCC LXXX VIII 3888
