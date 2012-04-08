#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include "files.h"
#include <pthread.h>
#include "../msg.h"
#include "league.h"
#include <unistd.h>
#include <sys/shm.h>
#include "externvars.h"
#include "clientAttendant.c"
#include "cmp.h"
#include "newMatchesListener.h"
#include <signal.h>
#include <time.h>
#include "getter.h"
#include "../colors.h"

void sighandler(int sig);
void * listenClient();
void defChannelListener();
void* save();
void* print();
void quitDraft(int msg);
void newClientAssist();
void clientAlive(int msg);
void clientDisconnect(int msg);
league_t** leagues;
int lCant, uCant;
user_t** users;
listADT clients;
strQueue_t printQueue;

int nextClientID=2;
void* channel;

int main()
{
	signal(SIGABRT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGINT, sighandler);
	clients=newList(cmpClient);
	loadAll();
	printQueue=newQueue();
	pthread_t saveThread,clThread,newMatchFilesThread, printThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	pthread_create(&printThread, NULL, print, NULL);
	//pthread_create(&saveThread, NULL, save, NULL);
	//pthread_create(&newMatchFilesThread, NULL, newMatchesListener, NULL);
	pthread_join(clThread, NULL);
	pthread_join(printThread, NULL);
	//pthread_join(saveThread, NULL);
	//pthread_join(newMatchFilesThread, NULL);
	return 0;
}

void * save()
{
	while(1)
	{
		saveAll();
		queueStr(printQueue,"Auto saved\n");
		sleep(30);
	}
}

void * listenClient()
{
	queueStr(printQueue,GREEN"Listening to clients...\n"WHITE);
	createChannel(DEFAULTID);
	channel=connectChannel(DEFAULTID);
	defChannelListener();
	return NULL;
}


void defChannelListener()
{
	while(1)
	{
		int msg;
		int bytes;
		bytes=rcvMsg(channel, (void*)&msg, sizeof(int));
		if(bytes>0)
		{
			if(msg==NEWCLIENT)
			{
				newClientAssist();
			}
			else if(msg>CLIENT_ALIVE && msg<QUIT_DRAFT)
			{
				clientAlive(msg);
			}
			else if(msg<CLIENT_DISCONNECT )
			{
				clientDisconnect(msg);
			}
			else if(msg>QUIT_DRAFT)
			{
				quitDraft(msg);
			}
			else
			{
				queueStr(printQueue,RED"Wrong CMD %d\n"WHITE, msg);
			}
		}
	}
}

void quitDraft(int msg)
{
	int clientID=(msg-QUIT_DRAFT);
	client_t* clientQ=getClientByID(clientID);
	league_t* myLeague=leagues[clientQ->user->draftLeague];
	team_t* team = getTeamByClient(myLeague, clientQ);
	myLeague->draft->clients[team->ID]=NULL;
	sem_post(myLeague->draft->sem[team->ID]);
	sem_destroy(myLeague->draft->sem[team->ID]);
	myLeague->draft->sem[team->ID]=NULL;
	if(myLeague->draft->turn==team->ID)
	{
		myLeague->draft->sent=0;
	}
	queueStr(printQueue,RED"Client %d quit Draft\n"WHITE, clientQ->ID);
}

void newClientAssist()
{
	int id= nextClientID;
	nextClientID+=2;
	createChannel(id);
	createChannel(id+1);
	sndMsg(channel, (void*)&id, sizeof(int));
	queueStr(printQueue,GREEN"New client connected. Asigned ID %d\n"WHITE,id);
	client_t* newClient = malloc(sizeof(client_t));
	newClient->ID=id;
	newClient->time=time(NULL);
	newClient->user=NULL;
	insert(clients, newClient);
	pthread_create(&(newClient->att), NULL, clientAtt, (void*) newClient);
}

void clientAlive(int msg)
{
	int clientID=msg-CLIENT_ALIVE;
	client_t* client;
	client=getClientByID(clientID);
	client->time=time(NULL);
}

void clientDisconnect(int msg)
{
		printf("entre al Client disc \n");

	int clientID=-msg+CLIENT_DISCONNECT;
	client_t* client;
	client=getClientByID(clientID);
	makeDisconnection(client);
}

void sighandler(int sig)
{
    client_t * client;
    reset(clients);
    disconnect(channel);
    destroyChannel(DEFAULTID);
    while(((client=getNext(clients)))!=NULL){
    	if(client->channel !=NULL){
    		disconnect(client->channel);
    		destroyChannel(client->ID);
    		destroyChannel(client->ID+1);
    	}
    }
    char* semN="semPrint";
    sem_unlink(semN);
    printf("\n Destroyed channels\n");
    exit(0);
}

void* print()
{
	while(1)
	{
		printf("%s",dequeueStr(printQueue));
	}
}
