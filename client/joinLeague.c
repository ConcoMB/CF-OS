#include "../common.h"
#include "../msg.h"
#include <stdlib.h>
#include "connection.h"
#include <stdio.h>

int main(int argc, char** args)
{
    int msg = JOIN_LEAGUE;
	int lID, clientID;
	char name[NAME_LENGTH];
    void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    lID=atoi(args[2]);
    sndMsg(channel, (void*)&msg, sizeof(int));
    sndMsg(channel, (void*)&lID, sizeof(int));
    rcvMsg(channel, (void*)&msg, sizeof(int));
    if(msg==ID_INVALID)
    {
    	printf("Invalid league ID\n");
    }
    else if(msg==USER_ALREADY_JOINED)
    {
    	printf("You already have a team in that league!\n");
    }
    else if(msg==JOIN_LEAGUE)
    {
    	do
    	{
    		printf("Enter your team name\n");
    		scanf("%s", name);
    		sndString(channel, name);
   			rcvMsg(channel, (void*)&msg, sizeof(int));
   			if(msg==NAME_TAKEN)
   			{
   				printf("Name taken, try an other one\n");
   			}
   		}
   		while(msg!=JOIN_LEAGUE);
   		rcvMsg(channel, (void*)&msg, sizeof(int));
   		if(msg==SEND_PASSWORD)
   		{
   			printf("Enter password\n");
   			scanf("%s", name);
    		sndString(channel, name);
    	}
   		rcvMsg(channel, (void*)&msg, sizeof(int));
   		if(msg==INCORRECT_PASSWORD)
   		{
   			printf("Incorrect password\n");
   		}
      else if(msg==LEAGUE_FULL)
      {
        printf("The league is full\n");
      }
   		else if(msg==JOIN_LEAGUE)
   		{
   			printf("You joined the league!\n");
   		}
    }
    exit(0);
}
