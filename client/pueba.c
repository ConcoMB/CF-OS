#include "printer.h"
#include <stdio.h>

int main()
{
	int i;
	int m, u, v ,t;
	char s[100];
	initWindow();
	while(1)
	{
		scanw("%d %d %d %d %s",&m, &u, &v, &t, s);
		//scanw("%d",&m);
		printString("hola %d %d %d %d %s",m, u, v, t, s);
	}
	return 0;
}
