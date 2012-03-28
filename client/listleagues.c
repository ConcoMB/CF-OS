#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"

int main(int argc, char** args)
{
    int msg = SEND_LEAGUE;
    char name[NAME_LENGTH];
    int id, clientID;
	clientID=atoi(args[1]);
    void* channel;
	connectClient(clientID,&channel);
    sndMsg(channel, (void*)&msg, sizeof(int));
    rcvMsg(channel, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_LEAGUE && msg==SEND_LEAGUE)
    {
	   rcvString(channel, name);
	   printf("Liga %s", name);
	   rcvMsg(channel, (void*)&id, sizeof(int));
	   printf("-> ID: %d\n",id);
	   rcvMsg(channel, (void*)&msg, sizeof(int));
    }

    exit(0);
}

