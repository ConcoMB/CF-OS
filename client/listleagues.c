#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
    int msg = SEND_LEAGUE;
    char name[NAME_LENGTH];
    int id;
    char readChannel[4], writeChannel[4];
    strcpy(readChannel, args[2]);
    strcpy(writeChannel, args[1]);
    sndMsg(writeChannel, (void*)&msg, sizeof(int));
    rcvMsg(readChannel, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_LEAGUE && msg==SEND_LEAGUE)
    {
	rcvString(readChannel, name);
	printf("Liga %s", name);
	rcvMsg(readChannel, (void*)&id, sizeof(int));
	printf("-> ID: %d\n",id);
	rcvMsg(readChannel, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}
    