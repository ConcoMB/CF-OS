
#include <pthread.h>
#include <time.h>
#include "getter.h"
#include "../common.h"
#include "league.h"
#include "display.h"


int setRandomSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);


void * draftAttendant(void* arg1)
{
	draft_t* draft=(draft_t*) arg1;
	int way=1, step=0, msg, i;
	time_t start, now;
	double diff=0;
	client_t* client;
	draft->turn=0;
	for(i=0; i< draft->league->tMax; i++)
	{
		msg=DRAFT_BEGUN;
		sndMsg(draft->clients[i]->channel, (void*)&msg, sizeof(int));
	}
	while(step!=draft->league->tMax*TEAM_SIZE)
	{
		int i;
		pthread_t tReader;
		draft->flag=0;
		client=draft->clients[draft->turn];
		msg=YOUR_TURN;
		sndMsg(client->channel, (void*)&msg, sizeof(int));
		msg=DRAFT_WAIT;
		for(i=0; i<draft->league->tMax; i++)
		{
			if(i!=draft->turn)
			{
				sndMsg(draft->clients[i]->channel, (void*)&msg, sizeof(int));
			}
			else
			{
				sendAllSportists(draft->league,  client->channel, SEND_SPORTIST);
			}
		}
		pthread_create(&tReader, NULL, sportistReader, (void*) draft);
		start=time(NULL);
		while(diff<=5||!draft->flag)
		{
			pthread_cancel(tReader);
			now=time(NULL);
			diff=difftime(now, start);
			printf("%f\n", diff);
		}
		if(!draft->flag) //TIME ELLAPSED
		{
			team_t* team=getTeamByClient(draft->league, client );
			msg=setRandomSportist(draft->league, team);
			sndMsg(client->channel, (void*)&msg, sizeof(int));
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

int setRandomSportist(league_t* league, team_t* team)
{
	int i=0;
	for(i=0; i<CANT_SPORTIST; i++)
	{
		if(league->sportists[i]->team==NULL)
		{
			league->sportists[i]->team=team;
			return i;
		}
	}
}

void* sportistReader(void* arg1)
{
	int msg;
	draft_t* draft=(draft_t*) arg1;
	int id;
	client_t* client = draft->clients[draft->turn];

	do
	{
		rcvMsg(draft->clients[draft->turn]->channel, (void*)&msg, sizeof(int));
		if(id>=0 && id<CANT_SPORTIST && draft->league->sportists[id]->team!=NULL)
		{
			draft->flag=1;
			team_t* team=getTeamByClient(draft->league, draft->clients[draft->turn]);
			draft->league->sportists[id]->team=team;
			msg=DRAFT_OK;
			sndMsg(client->channel, (void*)&msg, sizeof(int));
			pthread_exit(0);
		}
		else
		{
			msg=ID_INVALID;
			sndMsg(client->channel, (void*)&msg, sizeof(int));
		}	
	}while(1);
}
