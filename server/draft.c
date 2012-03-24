#include <pthread.h>
#include <time.h>
#include "getter.h"
#include "../common.h"
#include "league.h"


void setRandomSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);


void * draftAttendant(void* arg1)
{
	draft_t* draft=(draft_t*) arg1;
	int way=1, step=0, msg, i;
	time_t start, now;
	double diff=0;
	client_t* client;
	for(i=0; i< draft->league->tMax; i++)
	{
		msg=DRAFT_BEGUN;
		sndMsg(draft->clients[i]->writeFD, (void*)&msg, sizeof(int));
	}
	while(step!=draft->league->tMax*TEAM_SIZE)
	{
		pthread_t tReader;
		draft->flag=0;
		client=draft->clients[draft->turn];
		msg=YOUR_TURN;
		sndMsg(client->writeFD, (void*)&msg, sizeof(int));
		pthread_create(&tReader, NULL, sportistReader, (void*) draft);
		start=time(NULL);
		while(diff<=5||!draft->flag)
		{
			now=time(NULL);
			diff=difftime(now, start);
		}
		if(!draft->flag)
		{
			team_t* team=getTeamByClient(draft->league, client );
			setRandomSportist(draft->league, team);
		}
		draft->turn+=way;
		if(step%TEAM_SIZE==0)
		{
			way*=-1;
		}
		step++;
	}
	draft->league->draft=NULL;
	free(draft);
	pthread_exit(0);
}

void setRandomSportist(league_t* league, team_t* team)
{
	int i=0;
	for(i=0; i<CANT_SPORTIST; i++)
	{
		if(league->sportists[i]->team==NULL)
		{
			league->sportists[i]->team=team;
			return;
		}
	}
}

void* sportistReader(void* arg1)
{
	int msg;
	draft_t* draft=(draft_t*) arg1;
	int id;
	rcvMsg(draft->clients[draft->turn]->readFD, (void*)&msg, sizeof(int));
	draft->flag=1;
	team_t* team=getTeamByClient(draft->league, draft->clients[draft->turn]);
	draft->league->sportists[id]->team=team;
	pthread_exit(0);	
}
