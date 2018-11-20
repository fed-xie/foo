int romanToInt(char* s)
{
	if (!s)
		return 0;
	
	int i = 0;
	while (*s) {
		switch (*s) {
		case 'M': i += 1000; break;
		case 'D': i += 500; break;
		case 'L': i += 50; break;
		case 'V': i += 5; break;
		case 'C':
			if ('M' == s[1] || 'D' == s[1])
				i -= 100;
			else
				i += 100;
			break;
		case 'X':
			if ('C' == s[1] || 'L' == s[1])
				i -= 10;
			else
				i += 10;
			break;
		case 'I':
			if ('X' == s[1] || 'V' == s[1])
				i -= 1;
			else
				i += 1;
		default:
			break;
		}
		++s;
	}
	return i;
}