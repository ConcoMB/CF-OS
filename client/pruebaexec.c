#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int i=1212;
	char s[12];
	sprintf(s, "%d", i);
	if(fork())
	{
		wait((int*)0);
	}
	else
	{	
		printf("soy el hijo\n");
		execl("./ex", "ex", s, NULL);
	}
	return 0;
}