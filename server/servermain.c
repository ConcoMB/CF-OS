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

void * listenClient();
void newClient();
league_t** leagues;
int lCant, uCant;
user_t** users;
listADT clients;
int nextUserID=0;
	char defWChannel[3], defRChannel[3];

int nextLeagueID=0;

int nextClientID=1;
int readFD, writeFD;

int main()
{
	clients=newList(cmpClient);
	loadAll();
	pthread_t clThread,newMatchFilesThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	pthread_create(&newMatchFilesThread, NULL, newMatchesListener, NULL);
	pthread_join(clThread, NULL);
	pthread_join(newMatchFilesThread, NULL);
	return 0;
}


void * listenClient()
{
	printf("listening to clients\n");
	sprintf(defWChannel, "%c%d", 's', DEFAULTID);
	sprintf(defRChannel, "%c%d", 'c', DEFAULTID);
	create(defWChannel);
	create(defRChannel);
	readFD=connect(defRChannel, O_RDONLY);
	writeFD=connect(defWChannel, O_WRONLY);
	signal(SIGPIPE, SIG_IGN);
	newClient();
}


void newClient()
{
	while(1)
	{
		sleep(2);
		int msg;
		int bytes;
		bytes=rcvMsg(readFD, (void*)&msg, sizeof(int));
		if(bytes>0){
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
}
