#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  int readFD, writeFD;
  readFD=atoi(args[2]);
  writeFD=atoi(args[1]);
  int ID=atoi(args[3]);
  int msg=DRAFT;
  sndMsg(writeFD, (void*)&msg, sizeof(int));
  sndMsg(writeFD, (void*)&ID, sizeof(int));
  rcvMsg(readFD, (void*)&msg, sizeof(int));
  if(msg==ID_INVALID)
  {
  	printf("INVALID ID\n");
  	exit(0);
  }
  if(msg==DRAFT_WAIT)
  {
  	printf("Waiting for other teams...\n");
  	rcvMsg(readFD, (void*)&msg, sizeof(int));
  	if(msg==DRAFT_BEGUN)
  	{
  		printf("Draft has begun\n");
	  	rcvMsg(readFD, (void*)&msg, sizeof(int));
	  	while(msg!=END_DRAFT)
	  	{
	  		if(msg==YOUR_TURN)
	  		{
	  			int spID;
	  			printf("Please choose your sportist: type its ID\n");
	  			scanf("%d", &spID);
	 			sndMsg(writeFD, (void*)&spID, sizeof(int));
	 			rcvMsg(readFD, (void*)&msg, sizeof(int));
	 			if(msg==ID_INVALID)
	 			{
	 				printf("Invalid ID\n");
	 				msg=YOUR_TURN;
	 			}
	 			else if(msg==DRAFT_WAIT)
	 			{
	 				printf("You now have your desired sportist\n");
	 				rcvMsg(readFD, (void*)&msg, sizeof(int));
	 			}
	  		}
	  		else
	  		{
	  			rcvMsg(readFD, (void*)&msg, sizeof(int));
	  		}
	  	}
	  }
  }

