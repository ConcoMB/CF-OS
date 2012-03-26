#include "../common.h"
#include "../msg.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** args)
{
    printf("entre a listrtades\n");
	int msg = SEND_TRADE, clientID;
	char string[50];
	void* readFD, *writeFD;
	clientID=atoi(args[1]);
	connectClient(clientID,&writeFD,&readFD);
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_TRADE && msg==SEND_TRADE)
    {
    	rcvString(readFD, string);
    	printf("%s\n", string);
    	rcvMsg(readFD, (void*)&msg, sizeof(int));
    }

    exit(0);
}
