
#include <pthread.h>
#include <time.h>
#include "getter.h"
#include "../common.h"
#include "league.h"
#include "display.h"
#include "../colors.h"


int setForcedSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);
int sendToClient(client_t* client, int msg);
void draftEnd(draft_t* draft);
void draftBegin(draft_t* draft);
void listenQuitters(draft_t* draft);


int sendToClient(client_t* client, int msg)
{
	if(client!=NULL)
	{
		return sndMsg(client->channel, (void*)&msg, sizeof(int));
	}
	else
	{
		printf("es null no le mando\n");
	}
	return -1;
}

void * draftAttendant(void* arg1)
{
	printf("DRAFT ATTENDANT STARTED\n");
	draft_t* draft=(draft_t*) arg1;
	int way=1, step=0, msg, i, leagueSize=draft->league->tMax;
	draft->diff=0;
	draft->turn=0;
	draft->flag=0;
	char semName[15];
	sprintf(semName,"/semDraft%d",draft->league->ID);
	draft->chooseSem=sem_open(semName, O_RDWR|O_CREAT, 0666, 0);
	//draftBegin(draft);
	for(i=0; i< draft->league->tMax; i++)
	{
		msg=DRAFT_BEGUN;
		sendToClient(draft->clients[i], msg);
	}
	while(step!=leagueSize*TEAM_SIZE)
	{
		int i;
		printf("----- TURN %d ------\n", draft->turn);
		pthread_t tReader;
		draft->flag=0;
		draft->diff=0;
		draft->sent=0;
		msg=DRAFT_WAIT;
		for(i=0; i<leagueSize; i++)
		{
			if(i!=draft->turn)
			{
				sendToClient(draft->clients[i], msg);
			}
		}
		if(draft->clients[draft->turn]!=NULL)
		{
			draft->sent=1;
			msg=YOUR_TURN;
			if(draft->clients[draft->turn]!=NULL)
				printf("Sending turn\n");
			sendToClient(draft->clients[draft->turn], msg);
				printf("Sending sportists\n");
			sendAllSportists(draft->league,  draft->clients[draft->turn]->channel, SEND_SPORTIST);
			draft->end=DRAFT_TIME;
			printf("Sending time\n");
			sndMsg(draft->clients[draft->turn]->channel, (void*)&(draft->end), sizeof(double));
		}
		pthread_create(&tReader, NULL, sportistReader, (void*) draft);
		draft->start=time(NULL);
		while(draft->diff<=DRAFT_TIME && !draft->flag)
		{
			draft->now=time(NULL);
			draft->diff=difftime(draft->now, draft->start);
		}
		pthread_cancel(tReader);
		if(draft->flag!=1) //TIME ELLAPSED
		{
			printf("Time Elapsed\n");
			team_t* team=draft->league->teams[draft->turn];
			msg=setForcedSportist(draft->league, team);
			sendToClient(draft->clients[draft->turn], msg);
		}
		else
		{
			msg=DRAFT_OK;
			sendToClient(draft->clients[draft->turn], msg);
		}
		
		draft->turn+=way;
		if(draft->turn==leagueSize)
		{
			draft->turn--;
			way*=-1;
		}
		else if(draft->turn==-1)
		{
			draft->turn++;
			way*=-1;
		}
		step++;
	}
	printf("DRAFT ENDED\n");
	msg=END_DRAFT;
	for(i=0; i<leagueSize; i++)
	{
		printf("Sending END_DRAFT to %d\n",i);
		sendToClient(draft->clients[i], msg);
		if(draft->sem[i])
		{
			sem_post(draft->sem[i]);
		}
	}
	draftEnd(draft);
	printf("1\n");
	draft->league->draft=NULL;
	printf("2\n");
	free(draft->clients);
	printf("3\n");
	free(draft);
	printf("4\n");
	pthread_exit(0);
}

void draftBegin(draft_t* draft)
{
	int i;
	for(i=0; i<draft->league->tMax; i++)
	{
		draft->league->teams[i]->user->draftLeague=draft->league->ID;
	}
}


void draftEnd(draft_t* draft)
{
	int i;
	for(i=0; i<draft->league->tMax; i++)
	{
		draft->league->teams[i]->user->draftLeague=-1;
	}
}

int setForcedSportist(league_t* league, team_t* team)
{
	int i=0;
	for(i=0; i<CANT_SPORTIST; i++)
	{
		if(league->sportists[i]->team==NULL)
		{
			league->sportists[i]->team=team;
			return i+league->ID*CONVERSION;
		}
	}
	return -1;
}

void* sportistReader(void* arg1)
{
	int msg;
	draft_t* draft=(draft_t*) arg1;
	int id, lID;
	printf(BLUE"SReader started\n"WHITE);
	do
	{
		sem_init(draft->chooseSem,0,0);
		if(draft->clients[draft->turn]==NULL)
		{
			sem_wait(draft->chooseSem);
		}
		rcvMsg(draft->clients[draft->turn]->channel, (void*)&id, sizeof(int));
		lID=id/CONVERSION;
		id%=CONVERSION;

		if(lID==draft->league->ID && id>=0 && id<CANT_SPORTIST && draft->league->sportists[id]->team==NULL)
		{
			printf(BLUE"Chosen sportist: %d\n"WHITE, id);
			draft->flag=1;
			team_t* team=getTeamByClient(draft->league, draft->clients[draft->turn]);
			draft->league->sportists[id]->team=team;
			pthread_exit(0);
		}
		else
		{
			msg=ID_INVALID;
			sendToClient(draft->clients[draft->turn], msg);
		}	
	}while(1);
}
