#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    int offer, change;
    printf("entre al fork\n");
    printf("please enter the ID of the sportists you want to change (first, your offer, then the one you want\n");
    scanf("%d", &offer);
    scanf("%d", &change);
    int msg = TRADE_NEG, clientID;
    int tradeID=atoi(args[2]);
    void* channel;
	clientID=atoi(args[1]);
	connectClient(clientID,&channel);
    sndMsg(channel, (void*)&msg, sizeof(int));
    sndMsg(channel, (void*)&tradeID, sizeof(int));
    sndMsg(channel, (void*)&offer, sizeof(int));
    sndMsg(channel, (void*)&change, sizeof(int));

    rcvMsg(channel, (void*)&msg, sizeof(int));
    if(msg==TRADE_NEG)
    {
    	printf("Trade negociated, waiting for its acceptance\n");
    }
    else if(msg==ERROR)
    {
    	printf("An error occurred\n");
    }
    else if(msg==ID_INVALID)
    {
    	printf("You entered an invalid ID, please check that you're playing"
    		"at the same league of the team whose player you want and assure that he has that player and"
    		"you have the one you offer\n");
    }
    exit(0);

}
