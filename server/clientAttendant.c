#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "join.h"
#include "league.h"
#include <sys/shm.h>
#include <signal.h>
#include "commands.h"
#include "externvars.h"
#include "../colors.h"

void makeConnection(client_t* myClient);
void start(client_t* myClient);
void logClient(client_t* myClient);
void makeDisconnection(client_t* myClient);
int controlDraft(draft_t* draft);
void* keepAlive(void* arg);
void setNullIfDraft(client_t* myClient);



void* clientAtt(void* arg)
{
	client_t* myClient;
	myClient=(client_t*)arg;
	makeConnection(myClient);
	pthread_create(&myClient->keepAliveThread, NULL, keepAlive, (void*)myClient);
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
		queueStr(printQueue,"msg: %d\n", msg);
		if(rcvString(myClient->channel, name)<=0)
		{
			makeDisconnection(myClient);
		}
		queueStr(printQueue,"name: %s\n", name);
		if(rcvString(myClient->channel, password)<=0)
		{
			makeDisconnection(myClient);
		}
		queueStr(printQueue,"psswd: %s\n", password);
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
	int msg;
	while(1)
	{
		queueStr(printQueue,MAGENTA"Attending %d\n"WHITE,myClient->ID);
		if(rcvMsg(myClient->channel,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		if(msg==LOG_OUT)
		{
			myClient->user=NULL;
			return;
		}
		queueStr(printQueue,WHITE"Message: %d (Client %d)\n",msg, myClient->ID);
		if(msg>=CMD_START)
		{
			cmds[msg-CMD_START](myClient);
		}
	}
}



void makeDisconnection(client_t* myClient)
{
	setNullIfDraft(myClient);
	disconnect(myClient->channel);
	destroyChannel(myClient->ID);
	destroyChannel(myClient->ID+1);
	queueStr(printQueue,RED"Client %d disconnected\n"WHITE,myClient->ID);
	pthread_cancel(myClient->keepAliveThread);
	delete(clients, myClient);
	free(myClient);
	pthread_cancel(myClient->att);
}

void setNullIfDraft(client_t* myClient)
{
	int aux;
	if(myClient->user!=NULL && (aux=myClient->user->draftLeague)!=-1)
	{
		client_t ** dClients=leagues[aux]->draft->clients;
		sem_t ** dSem=leagues[aux]->draft->sem;
		team_t * team = getTeamByClient(leagues[aux], myClient);
		if(dClients[team->ID]!=NULL)
		{
			dClients[team->ID]=NULL;
			sem_post(dSem[team->ID]);
			sem_destroy(dSem[team->ID]);
			dSem[team->ID]=NULL;
			if(leagues[aux]->draft->turn==team->ID)
			{
				leagues[aux]->draft->sent=0;
			}
			queueStr(printQueue,RED"Client %d taken out from Draft\n"WHITE, myClient->ID);
			return;
		}
	}
}

void makeConnection(client_t* myClient)
{
	int id=myClient->ID;
	myClient->channel=connectChannel(id);
}


void* keepAlive(void* arg)
{
	client_t* myClient;
	myClient=(client_t*)arg;
	while(1)
	{
		double diff=difftime(time(NULL), myClient->time);
		if(diff>12)
		{
			makeDisconnection(myClient);
		}
		sleep(9);
	}
}

