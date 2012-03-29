#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int flag;

void* spChooser(void* channel);

int main(int argc, char** args)
{
  time_t start, now, diff;
  int clientID;
  void* channel;
  clientID=atoi(args[1]);
  connectClient(clientID,&channel);
  int ID=atoi(args[2]);
  int msg=DRAFT;
  printf("%d\n", ID);
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
	  			int i;
	  			char string[200];
	  			for(i=0; i<CANT_SPORTIST; i++)
	  			{
		 			rcvMsg(channel, (void*)&msg, sizeof(int));
		 			rcvString(channel, string);
		 			printf("%s\n", string);
	  			}
	  			flag=0;
	  			pthread_t spChooserT;
	  			int spID;
	  			printf("Please choose your sportist: type its ID\n");
	  			pthread_create(&spChooserT, channel, spChooser, NULL);
	  			while(diff<=5||!flag)
				{
					pthread_cancel(tReader);
					now=time(NULL);
					diff=difftime(now, start);
					printf("%f\n", diff);
				}
				if(!flag) //NO SE ELIGIO
				{
		 			rcvMsg(channel, (void*)&msg, sizeof(int));
					printf("Time ellapsed, you have a random sportist, ID %d\n",msg);
				}
	  		}
	  		else if(msg==DRAFT_WAIT)
	  		{
	  			printf("The other players are picking teams, please wait.\n");
	  		}

	  		rcvMsg(channel, (void*)&msg, sizeof(int));

	  	}
	  }
  }
}

void* spChooser(void* channel)
{
	int spID, msg;
	while(1)
	{
		scanf("%d", &spID);
		sndMsg(channel, (void*)&spID, sizeof(int));
		rcvMsg(channel, (void*)&msg, sizeof(int));
		if(msg==ID_INVALID)
		{
			printf("Invalid sportists ID\n");
		}
		else if(msg==DRAFT_OK)
		{
			printf("You now have your desired sportist\n");
			flag=1;
			pthread_exit(0);
		}
	}
}
