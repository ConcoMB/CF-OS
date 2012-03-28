#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  int clientID;
  void* channel;
  clientID=atoi(args[1]);
  connectClient(clientID,&channel);
  int ID=atoi(args[2]);
  int msg=DRAFT;
  sndMsg(channel, (void*)&msg, sizeof(int));
  sndMsg(channel, (void*)&ID, sizeof(int));
  rcvMsg(channel, (void*)&msg, sizeof(int));
  if(msg==ID_INVALID)
  {
  	printf("INVALID ID\n");
  	exit(0);
  }
  if(msg==DRAFT_WAIT)
  {
  	printf("Waiting for other teams...\n");
  	rcvMsg(channel, (void*)&msg, sizeof(int));
  	if(msg==DRAFT_BEGUN)
  	{
  		printf("Draft has begun\n");
	  	rcvMsg(channel, (void*)&msg, sizeof(int));
	  	while(msg!=END_DRAFT)
	  	{
	  		if(msg==YOUR_TURN)
	  		{
	  			int spID;
	  			printf("Please choose your sportist: type its ID\n");
	  			scanf("%d", &spID);
	 			sndMsg(channel, (void*)&spID, sizeof(int));
	 			rcvMsg(channel, (void*)&msg, sizeof(int));
	 			if(msg==ID_INVALID)
	 			{
	 				printf("Invalid ID\n");
	 				msg=YOUR_TURN;
	 			}
	 			else if(msg==DRAFT_WAIT)
	 			{
	 				printf("You now have your desired sportist\n");
	 				rcvMsg(channel, (void*)&msg, sizeof(int));
	 			}
	  		}
	  		else
	  		{
	  			rcvMsg(channel, (void*)&msg, sizeof(int));
	  		}
	  	}
	  }
  }
}

