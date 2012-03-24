#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    printf("entre al fork\n");
    int msg = SEND_LEAGUE;
    char name[NAME_LENGTH];
    int id;
    int readFD, writeFD;
	readFD=atoi(args[2]);
	writeFD=atoi(args[1]);
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_LEAGUE && msg==SEND_LEAGUE)
    {
	   rcvString(readFD, name);
	   printf("Liga %s", name);
	   rcvMsg(readFD, (void*)&id, sizeof(int));
	   printf("-> ID: %d\n",id);
	   rcvMsg(readFD, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}
    