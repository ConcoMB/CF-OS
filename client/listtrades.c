#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = SEND_TRADE;
	char readChannel[4], writeChannel[4], string[50];
    strcpy(readChannel, args[2]);
    strcpy(writeChannel, args[1]);
    sndMsg(writeChannel, (void*)&msg, sizeof(int));
    rcvMsg(readChannel, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_TRADE && msg==SEND_TRADE)
    {
    	rcvString(readChannel, string);
    	printf("%s\n", string);
    	rcvMsg(readChannel, (void*)&msg, sizeof(int));
    }

    exit(0);
}