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

void sighandler(int sig);
void * listenClient();
void newClient();
void* save();
void* print();
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
	pthread_create(&newMatchFilesThread, NULL, newMatchesListener, NULL);
	pthread_join(clThread, NULL);
	pthread_join(printThread, NULL);
	//pthread_join(saveThread, NULL);
	pthread_join(newMatchFilesThread, NULL);
	return 0;
}

void * save()
{
	while(1)
	{
		queueStr(printQueue,"Saving...");
		saveAll();
		queueStr(printQueue,"OK\n");
		sleep(30);
	}
}

void * listenClient()
{
	queueStr(printQueue,"Listening to clients\n");
	createChannel(DEFAULTID);
	channel=connectChannel(DEFAULTID);
	newClient();
	return NULL;
}


void newClient()
{
	while(1)
	{
		sleep(2);
		int msg;
		int bytes;
		bytes=rcvMsg(channel, (void*)&msg, sizeof(int));
		if(bytes>0)
		{
			switch(msg)
			{
				case NEWCLIENT:
					queueStr(printQueue,"Sending msgid...");
					int id= nextClientID;
					nextClientID+=2;
					createChannel(id);
					createChannel(id+1);
					sndMsg(channel, (void*)&id, sizeof(int));
					queueStr(printQueue,"OK\n");
					fflush(stdout);
					client_t* newClient = malloc(sizeof(client_t));
					newClient->ID=id;
					newClient->time=time(NULL);
					insert(clients, newClient);
					pthread_create(&(newClient->att), NULL, clientAtt, (void*) newClient);
					break;
				default:
					if(msg>CLIENT_ALIVE)
					{
						int clientID=msg-CLIENT_ALIVE;
						client_t* client;
						client=getClientByID(clientID);
						client->time=time(NULL);
					}
					else
					{
<<<<<<< HEAD
						queueStr(printQueue,"Wrong CMD\n");
=======
						printf("Wrong CMD %d\n", msg);
>>>>>>> c8c09fadf8f4726cf9ab761a4663facd7f159a94
					}
					break;
			}
		}
	}
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
    	}
    }
    exit(0);
}

void* print()
{
	while(1)
	{
		printf("%s",dequeueStr(printQueue));
	}
}
