#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "server.h"
#include "league.h"
#include <unistd.h>

void * listenClient();


int main()
{
	clients=newList(cmpClient);
	connect(DEFAULTID);
	pthread_t clThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	pthread_join(clThread, NULL);
	return 0;
}

void * listenClient()
{
	printf("entre al thread\n");
	while(1)
	{
		int msg;
		rcvMsg(DEFAULTID, (void*)&msg, sizeof(int));
		printf("recibi %d\n", msg);
		if(msg==NEWCLIENT)
		{
			if(fork())
			{
				int id= nextClientID++;
				sndMsg(DEFAULTID, (void*)&id, sizeof(int));
				client_t* newClient = malloc(sizeof(client_t));
				newClient->ID=id;
				insert(clients, newClient);
				printf("me forkeo, msgid %d\n", id);
				execl("./clientAttendant", "clientAttendant",(char*)&id, (char*)0);
			}
		}
	}
}
