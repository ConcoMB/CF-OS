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
#include "trade.h"
#include <sys/shm.h>
#include <signal.h>
#include "draft.c"
#include "commands.h"

void makeConnection();
void start();
void logClient();
void makeDisconnection();
int controlDraft(draft_t* draft);


void* clientAtt(void* arg)
{
	client_t* myClient;
	myClient=(client_t*)arg;
	makeConnection(myClient);
	while(1)
	{
		logClient(myClient);
	}
}

void logClient(client_t* myClient)
{
	int aux, loged=0, msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	while(!loged)
	{
		if(rcvMsg(myClient->channel,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		printf("msg: %d\n", msg);
		if(rcvString(myClient->channel, name)<=0)
		{
			makeDisconnection(myClient);
		}
		printf("name: %s\n", name);
		if(rcvString(myClient->channel, password)<=0)
		{
			makeDisconnection(myClient);
		}
		printf("psswd: %s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password, myClient))==0)
			{
				loged=1;
			}
			sndMsg(myClient->channel, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			aux=signUp(name, password);
			sndMsg(myClient->channel, (void*)&aux, sizeof(int));
			if(aux==0)
			{
			    loged=1;
			    logIn(name, password, myClient);
			}
		}
	}
	start(myClient);
}

void start(client_t* myClient)
{
	int msg, lID, tID, offer, change;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	team_t * team;
	trade_t* trade;
	while(1)
	{
		sleep(2);
		if(rcvMsg(myClient->channel,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		cmds[msg-CMD_START](myClient);
	}
}

void makeDisconnection(client_t* myClient)
{
	disconnect(myClient->channel);
	printf("cliente desconectado\n");
	pthread_exit(0);
}

void makeConnection(client_t* myClient)
{
	int id=myClient->ID;
	myClient->channel=connectChannel(id);
}

int controlDraft(draft_t* draft)
{
	int i;
	for(i=0; i< (draft->league->tMax) ;i++)
	{
		if(!draft->clients[i])
		{
			return 0;
		}
	}
	return 1;
}

