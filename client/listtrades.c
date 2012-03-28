#include "../common.h"
#include "../msg.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args)
{
    printf("entre a listrtades\n");
	int msg = SEND_TRADE, clientID;
	char string[50];
	void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    sndMsg(channel, (void*)&msg, sizeof(int));
    rcvMsg(channel, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_TRADE && msg==SEND_TRADE)
    {
    	rcvString(channel, string);
    	printf("%s\n", string);
    	rcvMsg(channel, (void*)&msg, sizeof(int));
    }

    exit(0);
}
