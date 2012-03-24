#include <pthread.h>
#include <time.h>
#include "getter.h"

void setRandomSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);


void * draftAttendant(void* arg1)
{
	draft_t* draft=(draft_t*) arg1;
	int way=1, step=0, msg, i;
	time_t start, now;
	double diff=0;
	client_t* client;
	for(i=0;i<draft->league->tMax;i++)
	{
		msg=DRAFT_BEGUN;
		sndMsg(draft->clients[i]->writeFD, (void*)&msg, sizeof(int));
	}
	while(step!=league->tMax*TEAM_SIZE)
	{
		draft->chosen=0;
		client=clients[draft->turn];
		msg=YOUR_TURN;
		sndMsg(client->writeFD, (void*)&msg, sizeof(int));
		pthread_create(&tReader, NULL, sportistsReader, (void*) draft);
		start=time(NULL);
		while(diff<=5||!draft->chosen)
		{
			now=time(NULL);
			diff=difftime(now, start);
		}
		if(!draft->chosen)
		{
			team_t* team=getTeamByClient(draft->league, client );
			setRandomSportist(league, team);
		}
		draft->turn+=way;
		if(step%TEAM_SIZE==0)
		{
			way*=-1;
		}
		step++;
	}
	league->draft=NULL;
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
	draft_t* =(draft_t*) arg1;
	int id;
	rcvMsg(draft->clients[turn]->readFD, (void*)&msg, sizeof(int));
	draft->chosen=1;
	team_t* team=getTeamByClient(draft->league, draft->clients[turn]);
	draft->league->sportists[id]->team=team;
	pthread_exit(0);	
}
