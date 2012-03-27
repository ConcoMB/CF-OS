#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    int offer, change;
    printf("Enter the ID of your offered sportist, followed by the one you want in exchange\n");
    scanf("%d", &offer);
    scanf("%d", &change);
    int msg = MAKE_TRADE;
    int toID, clientID;
    void* readFD, *writeFD;
	clientID=atoi(args[1]);
	connectClient(clientID,&writeFD,&readFD);
	toID=atoi(args[2]);
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    sndMsg(writeFD, (void*)&toID, sizeof(int));
    sndMsg(writeFD, (void*)&offer, sizeof(int));
    sndMsg(writeFD, (void*)&change, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    if(msg==TRADE_OFFERED)
    {
    	printf("Trade made, waiting for its acceptance\n");
    }
    else if(msg==TRADE_MADE)
    {
        printf("Trade made\n");
    }
    else if(msg==ERROR)
    {
    	printf("An error occurred\n");
    }
    else if(msg==ID_INVALID)
    {
    	printf("You entered an invalid ID, please check that you're playing "
    		"at the same league of the team whose player you want and assure that he has that player and "
    		"you have the one you offer\n");
    }
    exit(0);

}
