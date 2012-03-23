#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
    int msg = SEND_TEAM;
    char teamName[NAME_LENGTH], leagueName[NAME_LENGTH];
    int readFD, writeFD;
	readFD=(int)args[2];
	writeFD=(int) args[1];
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    while(msg==SEND_TEAM && msg!=END_SEND_TEAM)
    {
	   rcvString(readFD, teamName);
	   rcvString(readFD, leagueName);
	   rcvMsg(readFD, (void*)&msg, sizeof(int));
	   printf("You own the team %s in the league %s, you currently hace %d points\n", teamName, leagueName, msg);
	   rcvMsg(readFD, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}