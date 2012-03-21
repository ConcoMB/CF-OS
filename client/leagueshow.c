#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = LEAGUE_SHOW;
	char readChannel[4], writeChannel[4];
    strcpy(readChannel, args[2]);
    strcpy(writeChannel, args[1]);
    sndMsg(writeChannel, (void*)&msg, sizeof(int));
    
    rcvMsg(readChannel, (void*)&msg, sizeof(int));
}