#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = SEND_TRADE;
	char string[50];
	int readFD, writeFD;
	readFD=(int)args[2];
	writeFD=(int) args[1];
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