#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
    int msg = SEND_TEAM;
    char teamName[NAME_LENGTH], leagueName[NAME_LENGTH];
    int read, write;
	read=(int)args[2];
	write=(int) args[1];
    sndMsg(write, (void*)&msg, sizeof(int));
    rcvMsg(read, (void*)&msg, sizeof(int));
    while(msg==SEND_TEAM && msg!=END_SEND_TEAM)
    {
	   rcvString(read, teamName);
	   rcvString(read, leagueName);
	   rcvMsg(read, (void*)&msg, sizeof(int));
	   printf("You own the team %s in the league %s, you currently hace %d points\n", teamName, leagueName, msg);
	   rcvMsg(read, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}