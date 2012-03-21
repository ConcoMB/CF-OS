#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
    int msg = SEND_TEAM;
    char readChannel[4], writeChannel[4], teamName[NAME_LENGTH], leagueName[NAME_LENGTH];
    strcpy(readChannel, args[2]);
    strcpy(writeChannel, args[1]);
    sndMsg(writeChannel, (void*)&msg, sizeof(int));
    rcvMsg(readChannel, (void*)&msg, sizeof(int));
    while(msg==SEND_TEAM && msg!=END_SEND_TEAM)
    {
	   rcvString(readChannel, teamName);
	   rcvString(readChannel, leagueName);
	   rcvMsg(readChannel, (void*)&msg, sizeof(int));
	   printf("You own the team %s in the league %s, you currently hace %d points\n", teamName, leagueName, msg);
	   rcvMsg(readChannel, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}