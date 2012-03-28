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

void sighandler(int sig);
void * listenClient();
void newClient();
league_t** leagues;
int lCant, uCant;
user_t** users;
listADT clients;

int nextClientID=2;
void* channel;

int main()
{
	clients=newList(cmpClient);
	loadAll();
	pthread_t clThread,newMatchFilesThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	//pthread_create(&newMatchFilesThread, NULL, newMatchesListener, NULL);
	pthread_join(clThread, NULL);
	//pthread_join(newMatchFilesThread, NULL);
	return 0;
}


void * listenClient()
{
	printf("listening to clients\n");
	createChannel(DEFAULTID);
	channel=connectChannel(DEFAULTID);

	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
	newClient();
}


void newClient()
{
	while(1)
	{
		sleep(2);
		int msg;
		int bytes;
		bytes=rcvMsg(channel, (void*)&msg, sizeof(int));
		if(bytes>0){
			printf("recibi %d\n", msg);
			if(msg==NEWCLIENT)
			{
				printf("sending msgid...");
				int id= nextClientID;
				nextClientID+=2;
				createChannel(id);
				createChannel(id+1);
				sndMsg(channel, (void*)&id, sizeof(int));
				printf("OK\n");
				fflush(stdout);
				client_t* newClient = malloc(sizeof(client_t));
				newClient->ID=id;
				insert(clients, newClient);
				pthread_create(&(newClient->att), NULL, clientAtt, (void*) newClient);
			}
			else
			{
				printf("wrong cmd\n");
			}
		}
	}
}

void sighandler(int sig)
{
    reset(clients);
    client_t * client;
    while((getNext(clients))!=NULL){
    	disconnect(client->channel);
    }
}
