#include "../common.h"
#include "../colors.h"
#include "../msg.h"
#include <stdio.h>
#include <pthread.h>
#include "connection.h"
#include <time.h>
#include <stdlib.h>

int flag;
int clientID;

void * quitThread(void* channel);
void* spChooser(void* channels);

int main(int argc, char** args)
{
	pthread_t quitT;
  time_t start, now;
  double diff=0, end;
  void* channels[2];
  clientID=atoi(args[1]);
  connectClient(clientID,&channels[0]);
  channels[1]=connectChannel(DEFAULTID+1);
  int ID=atoi(args[2]);
  int msg=DRAFT;
  sndMsg(channels[0], (void*)&msg, sizeof(int));
  sndMsg(channels[0], (void*)&ID, sizeof(int));
  rcvMsg(channels[0], (void*)&msg, sizeof(int));
  pthread_create(&quitT, NULL, quitThread, channels[1]);
  if(msg==ID_INVALID)
  {
  	printf("INVALID ID\n");
  	exit(0);
  }
  if(msg==DRAFT_WAIT)
  {
  	printf(BLUE"Waiting for other teams...\n"WHITE);
  	rcvMsg(channels[0], (void*)&msg, sizeof(int));
  	if(msg==DRAFT_BEGUN)
  	{
  		printf(GREEN"Draft has begun\n"WHITE);
	  	rcvMsg(channels[0], (void*)&msg, sizeof(int));
	  	while(msg!=END_DRAFT)
	  	{
	  		if(msg==YOUR_TURN)
	  		{
	  			diff=0;
	  			printf(RED"Its your turn to pick!!\n"WHITE);
	  			sleep(1);
	  			int i;
	  			char string[200];
	  			pthread_cancel(quitT);
	  			for(i=0; i<CANT_SPORTIST; i++)
	  			{
		 			rcvMsg(channels[0], (void*)&msg, sizeof(int));
		 			rcvString(channels[0], string);
		 			printf("/t%s", string);
	  			}
	  			flag=0;
	  			pthread_t sportThrd;
	  		  	rcvMsg(channels[0],(void*)&end, sizeof(double));
	  			start=time(NULL);
	  			//printf("%f tiempo \n", end);
	  			pthread_create(&sportThrd, NULL, spChooser, channels);
	  			while(diff<=end && !flag)
				{
					now=time(NULL);
					diff=difftime(now, start);
				}
				//pthread_join(sportThrd, NULL);
				pthread_cancel(sportThrd);
				if(!flag) //NO SE ELIGIO
				{	
		 			rcvMsg(channels[0], (void*)&msg, sizeof(int));
					printf(MAGENTA"Time ellapsed, you have a random sportist, ID %d\n"WHITE,msg);
				}
				pthread_create(&quitT, NULL, quitThread, channels[1]);

	  		}
	  		else if(msg==DRAFT_WAIT)
	  		{
	  			printf(BLUE"The other players are picking teams, please wait.\n"WHITE);
	  		}
	  		rcvMsg(channels[0], (void*)&msg, sizeof(int));
	  	}
	  	printf(GREEN"Draft ended\n"WHITE);
	  }
  }
  exit(0);
}

void* spChooser(void* arg)
{
	void** channels=(void**)arg;
	int msg;
	char string[10];
	while(1)
	{
		printf(CYAN"Please choose your sportist by ID: "WHITE);	
		scanf("%s", string);
		if(strcmp(string, "quit")==0)
		{
			msg=QUIT_DRAFT+clientID;
			sndMsg(channels[1], (void*)&msg, sizeof(int));
			exit(0);
		}
		msg=atoi(string);
		fflush(stdout);
		sndMsg(channels[0], (void*)&msg, sizeof(int));
		rcvMsg(channels[0], (void*)&msg, sizeof(int));
		if(msg==ID_INVALID)
		{
			printf(RED"Invalid sportists ID\n"WHITE);
		}
		else if(msg==DRAFT_OK)
		{
			printf(GREEN"You now have your desired sportist\n"WHITE);
			flag=1;
			pthread_exit(0);
		}
	}
	return NULL;
}

void * quitThread(void* channel)
{
	char  string[10];
	int msg;
	while(1)
	{
		scanf("%s", string);
		if(strcmp(string,"quit")==0)
		{
			msg=QUIT_DRAFT+clientID;
			sndMsg(channel, (void*)&msg, sizeof(int));
			exit(0);
		}
	}
	return NULL;
}
