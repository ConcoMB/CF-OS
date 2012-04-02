#include "printer.h"
#include <stdio.h>

int main()
{
	int i;
<<<<<<< HEAD
	char s[23];
=======
	int m, u, v ,t;
	char s[100];
>>>>>>> 2d494bed509f90b6f32a46332d25b4fcc69566a1
	initWindow();
	while(1)
	{
		scanw("%d %d %d %d %s",&m, &u, &v, &t, s);
		//scanw("%d",&m);
		printString("hola %d %d %d %d %s",m, u, v, t, s);
	}
	return 0;
}
