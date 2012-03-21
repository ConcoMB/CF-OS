#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "join.h"
#include "display.h"
#include "league.h"
#include <sys/shm.h>

char readChannel[4], writeChannel[4];

void makeConnection(int id);
void start();
void logClient();

void* clientAtt(void* arg)
{
	client_t* myClient=(client_t*)arg;
	makeConnection(myClient->ID);
	logClient();
	start(myClient);
}

void logClient()
{
	int aux, loged=0, msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	while(!loged)
	{
		rcvMsg(readChannel,(void*)&msg, sizeof(int));
		printf("%d\n", msg);
		rcvString(readChannel, name);
		printf("%s\n", name);
		rcvString(readChannel, password);
		printf("%s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password))==0)
			{
				loged=1;
			}
			sndMsg(writeChannel, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			aux=signUp(name, password);
			sndMsg(writeChannel, (void*)&aux, sizeof(int));
			if(aux==0)
			{
			    loged=1;
			    printf("voy al login\n");
			    logIn(name, password);
			}
		}
	}
}

void start(client_t* myClient)
{
	int msg;
	while(1)
	{
		rcvMsg(readChannel,(void*)&msg, sizeof(int));
		if(msg==SEND_LEAGUE)
		{
		     listLeagues(writeChannel);
		}
		else if(msg==SEND_TEAM)
		{
		    listTeam(myClient->user ,writeChannel);
		}
	}
}

void makeConnection(int id)
{
	sprintf(writeChannel, "%c%d", 's', id);
	connect(writeChannel);
	sprintf(readChannel, "%c%d", 'c', id);
	connect(readChannel);
}