#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "joining.h"
#include "league.h"
#include <sys/shm.h>

void* clientAtt(void* arg)
{
	int msg, aux, loged=0;
	int id= *(int*)arg;
	printf("id %d\n",id );
	char name[NAME_LENGTH], password[NAME_LENGTH];
	char channelS[4];
	sprintf(channelS, "%c%d", 's', id);
	connect(channelS);
	char channelC[4];
	sprintf(channelC, "%c%d", 'c', id);
	connect(channelC);
	
	while(!loged)
	{
		rcvMsg(channelC,(void*)&msg, sizeof(int));
		printf("%d\n", msg);
		rcvString(channelC, name);
		printf("%s\n", name);
		rcvString(channelC, password);
		printf("%s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password))!=0)
			{
				sndMsg(channelS, (void*)&aux, sizeof(int));
			}
			else
			{
				loged=1;
			}
		}
		else if(msg==SIGNUP)
		{
			if((aux=signUp(name, password))!=0)
			{
			//error
				sndMsg(channelS, (void*)&aux, sizeof(int));
			}
			loged=1;
			printf("voy al login\n");
			logIn(name, password);
		}
	}
	while(1)
	{
		rcvMsg(channelC,(void*)&msg, sizeof(int));
		//ifelse
	}
}