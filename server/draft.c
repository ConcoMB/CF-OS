
#include <pthread.h>
#include <time.h>
#include "getter.h"
#include "../common.h"
#include "league.h"
#include "display.h"


int setForcedSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);


void * draftAttendant(void* arg1)
{
	printf("entre a la funcion draft\n");
	draft_t* draft=(draft_t*) arg1;
	int way=1, step=0, msg, i, leagueSize=draft->league->tMax;
	time_t start, now;
	double diff=0;
	client_t* client;
	draft->turn=0;
	for(i=0; i< draft->league->tMax; i++)
	{
		msg=DRAFT_BEGUN;
		sndMsg(draft->clients[i]->channel, (void*)&msg, sizeof(int));
	}
	while(step!=leagueSize*TEAM_SIZE)
	{
		int i;
		pthread_t tReader;
		draft->flag=0;
		diff=0;
		client=draft->clients[draft->turn];
		msg=YOUR_TURN;
		sndMsg(client->channel, (void*)&msg, sizeof(int));
		msg=DRAFT_WAIT;
		for(i=0; i<leagueSize; i++)
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
		while(diff<=DRAFT_TIME && !draft->flag)
		{
			now=time(NULL);
			diff=difftime(now, start);
		}
		if(!draft->flag) //TIME ELLAPSED
		{
			printf("se te paso el tiempo\n");
			pthread_cancel(tReader);
			team_t* team=getTeamByClient(draft->league, client );
			msg=setForcedSportist(draft->league, team);
			sndMsg(client->channel, (void*)&msg, sizeof(int));
		}
			//pthread_join(tReader, NULL);
		
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
	draft->league->draft=NULL;
	msg=END_DRAFT;
	for(i=0; i<leagueSize; i++)
	{
		sndMsg(draft->clients[i]->channel, (void*)&msg, sizeof(int));
	}
	draft->league->draft=NULL;
	free(draft->clients);
	free(draft);
	pthread_exit(0);
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
	printf("turno %d\n", draft->turn);
	fflush(stdout);
	client_t* client = draft->clients[draft->turn];

	do
	{
		printf("do while\n");
		fflush(stdout);
		printf("antes del rcvmsg draft.c\n");
		fflush(stdout);
		rcvMsg(draft->clients[draft->turn]->channel, (void*)&id, sizeof(int));
		printf("dsps del rcvmsg draft.c\n");
		fflush(stdout);
		lID=id/CONVERSION;
		id%=CONVERSION;

		if(lID==draft->league->ID && id>=0 && id<CANT_SPORTIST && draft->league->sportists[id]->team==NULL)
		{
			printf("entre\n");
							fflush(stdout);

			draft->flag=1;
			team_t* team=getTeamByClient(draft->league, draft->clients[draft->turn]);
			draft->league->sportists[id]->team=team;
			msg=DRAFT_OK;
			sndMsg(client->channel, (void*)&msg, sizeof(int));
			pthread_exit(0);
		}
		else
		{
			printf("else\n");
							fflush(stdout);

			msg=ID_INVALID;
			sndMsg(client->channel, (void*)&msg, sizeof(int));
		}	
	}while(1);
}
