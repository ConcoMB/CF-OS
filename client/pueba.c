#include "printer.h"
#include <stdio.h>

int main()
{
	int i;
	char s[23];
	initWindow();
	while(1)
	{
		scanString(s);
		printString(s);
	}
	return 0;
}