#include "../common.h"
#include "../msg.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args)
{
	char lName[NAME_LENGTH], pass[NAME_LENGTH];
	int msg = MAKE_LEAGUE;
	int cantT, clientID;
    void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    char c[3];
	printf("Enter the league name\n");
	scanf("%s", lName);
	do
	{
		printf("Do you want it to be private? (y/n)\n");
		scanf("%s",c);

	}while(c[0]!='y' && c[0]!='n');

	if(c[0]=='y')
	{
		printf("Enter the password\n");
		scanf("%s", pass);
	}
	else
	{
		pass[0]=0;
	}
	printf("How many teams do you want in your league (3-10)\n");
	do
	{
		scanf("%d",&cantT);
		printf("lei %d\n", cantT);
	}while(!(cantT<=10 && cantT>=3));

	sndMsg(channel, (void*)&msg, sizeof(int));
	sndString(channel, lName);
	sndString(channel, pass);
	sndMsg(channel, (void*)&cantT, sizeof(int));
	rcvMsg(channel, (void*)&msg, sizeof(int));
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
		printf("League full, join an other one\n");
	}
	exit(0);
}
