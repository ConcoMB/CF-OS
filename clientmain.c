#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int login(char* name, char* pass){
	int fdsend, fdrcv;
	char buffer[5];
	mkfifo("./fifoc", 0666);
	fdsend=open("./fifoc", O_WRONLY);
	write(fdsend, name, 5);
	write(fdsend, pass, 5);
	fdrcv=open("./fifos", O_RDONLY);
	read("./fifos", buffer, 5);
	close(fdsend);
	close(fdrcv);
	if(strcmp(buffer, "si")==0)
	{
		return 1;
	}
	return 0;
}

int main()
{
	char nombre[5]={'c','a','c','o','\0'};
	char pass[5]={'c','u','c','a','\0'};

	if(login(nombre, pass)==1);{
		printf("YEAH\n");
	}
	return 0;
}