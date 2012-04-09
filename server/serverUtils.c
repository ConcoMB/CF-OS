#include "serverUtils.h"


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
    eraseDraftSems();
    sem_unlink("semPrint");
    sem_unlink("mutexSave");
    printf("\n Destroyed channels\n");
    exit(0);
}

void eraseDraftSems()
{
	int i;
	char semName[15];
	for(i=0; i<lCant; i++)
	{
		if(leagues[i]->draft!=NULL && leagues[i]->draft->flag!=-1)
		{
			sprintf(semName,"/semDraft%d",i);
			sem_unlink(semName);
		}
	}
}



void * save()
{
	while(1)
	{
		sleep(60);
		saveAll();
		queueStr(printQueue,"Auto saved\n");	
	}
	return NULL;
}

void * listenClient()
{
	queueStr(printQueue,GREEN"Listening to clients...\n"WHITE);
	createChannel(DEFAULTID);
	channel=connectChannel(DEFAULTID);
	defChannelListener();
	return NULL;
}


void* print()
{
	char* tmp;
	while(1)
	{
		tmp=dequeueStr(printQueue);
		printf("%s",tmp);
		free(tmp);
	}
}
