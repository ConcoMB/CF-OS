#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = SEND_TRADE;
	char string[50];
	int read, write;
	read=(int)args[2];
	write=(int) args[1];
    sndMsg(write, (void*)&msg, sizeof(int));
    rcvMsg(read, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_TRADE && msg==SEND_TRADE)
    {
    	rcvString(read, string);
    	printf("%s\n", string);
    	rcvMsg(read, (void*)&msg, sizeof(int));
    }

    exit(0);
}