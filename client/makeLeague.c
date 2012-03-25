#include "../common.h"
#include "../msg.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args)
{
	char lName[NAME_LENGTH], pass[NAME_LENGTH];
	int msg = MAKE_LEAGUE;
	int readFD, writeFD, cantT;
	readFD=atoi(args[2]);
    writeFD=atoi(args[1]);
    char c;
	printf("Enter the league name\n");
	scanf("%s", lName);
	do
	{
		printf("Do you want it to be private? (y/n)\n");
		scanf("%c",c);
	}while(c!='y' || c!='n')
	if(c=='y')
	{
		printf("Enter the password\n");
		scanf("%s", pass);
	}
	else
	{
		pass=NULL;
	}
	printf("How many teams do you want in your league (3-10)\n");
	do
	{
		scanf("%d",&canT)
	}while(canT<=10 && canT>=3);
	sndMsg(writeFD, (void*)&msg, sizeof(int));
	sndString(writeFD, lName);
	sndString(writeFD, pass);
	sndMsg(writeFD, (void*)&cantT, sizeof(int));
	rcvMsh(readFD, (void*)&msg, sizeof(int));
	if(msg==MAKE_LEAGUE)
	{
		printf("League created\n");
	}
	else if (msg==NAME_OCCUPIED)
	{
		printf("Name occupied, try an other\n");
	}
	else if(msg==LEAGUE_FULL)
	{
		printf("League full, join an other one
			\n");
	}
	exit(0);
}