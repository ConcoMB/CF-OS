#include "../common.h"
#include "../msg.h"
#include "connection.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args)
{
	char lName[NAME_LENGTH], pass[NAME_LENGTH];
	int msg = MAKE_LEAGUE;
	int cantT, clientID,flag;
    void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    char c[3];
    do
    {	
    	flag=0;
		printf("Enter the league name\n");
		scanf("%s", lName);
		if(strlen(lName)>=NAME_LENGTH)
		{
			printf("Name too long\n");
			flag=1;
		}
	}while(flag);
	do
	{
		printf("Do you want it to be private? (y/n)\n");
		scanf("%s",c);

	}while(c[0]!='y' && c[0]!='n');

	if(c[0]=='y')
	{
		do
		{
			printf("Enter the password\n");
			scanf("%s", pass);
			if(strlen(pass)>=NAME_LENGTH)
			{
				flag=1;
				printf("Password too long\n");
			}
		}while(flag);	
	}
	else
	{
		pass[0]=0;
	}
	do
	{
		printf("How many teams do you want in your league (3-10)\n");
		scanf("%d",&cantT);
	}while(!(cantT<=MAX_CANT_TEAM && cantT>=MIN_CANT_TEAM));

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
