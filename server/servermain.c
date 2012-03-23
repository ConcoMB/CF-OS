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
#include <signal.h>

void * listenClient();
void newClient();
league_t** leagues;
int lCant, uCant;
user_t** users;
listADT clients;
int nextUserID=0;
int nextLeagueID=0;

int nextClientID=1;
int readFD, writeFD;

int main()
{
	clients=newList(cmpClient);
	loadAll();
	pthread_t clThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	pthread_join(clThread, NULL);
	return 0;
}

void disconn()
{
	disconnect(readFD);
	disconnect(writeFD);
	listenClient();
}

void * listenClient()
{
	printf("listening to clients\n");
	char defWChannel[3], defRChannel[3];
	sprintf(defWChannel, "%c%d", 's', DEFAULTID);
	sprintf(defRChannel, "%c%d", 'c', DEFAULTID);
	create(defWChannel);
	create(defRChannel);
	readFD=connect(defRChannel, O_RDONLY);
	writeFD=connect(defWChannel, O_WRONLY);
	signal(SIGPIPE, disconn);
	newClient();
}


void newClient()
{
	while(1)
	{
		int msg;
	rcvMsg(readFD, (void*)&msg, sizeof(int));
	printf("recibi %d\n", msg);
	if(msg==NEWCLIENT)
	{
			int id= nextClientID++;
			sndMsg(writeFD, (void*)&id, sizeof(int));
			client_t* newClient = malloc(sizeof(client_t));
			newClient->ID=id;
			insert(clients, newClient);
			pthread_create(&(newClient->att), NULL, clientAtt, (void*) newClient);
	}
	}
}
