#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
    int msg = SEND_TEAM;
    char teamName[NAME_LENGTH], leagueName[NAME_LENGTH];
    int teamID,clientID;
    void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    sndMsg(channel, (void*)&msg, sizeof(int));
    rcvMsg(channel, (void*)&msg, sizeof(int));
    while(msg==SEND_TEAM && msg!=END_SEND_TEAM)
    {
	   rcvString(channel, teamName);
	   rcvString(channel, leagueName);
       rcvMsg(channel, (void*)&teamID, sizeof(int));
	   rcvMsg(channel, (void*)&msg, sizeof(int));
	   printf("You own the team %s (ID %d) in the league %s, you currently have %d points\n", teamName, teamID, leagueName, msg);
	   rcvMsg(channel, (void*)&msg, sizeof(int));
    }

    exit(0);
}
