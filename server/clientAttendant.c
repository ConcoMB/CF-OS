#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "join.h"
#include "display.h"
#include "league.h"
#include <sys/shm.h>
#define CONVERSION 1000
#include <signal.h>
#include "draft.c"

void makeConnection();
void start();
void logClient();
void makeDisconnection();
void controlDraft(draft_t* draft);


void* clientAtt(void* arg)
{
	client_t* myClient;
	printf("entre al attendant\n");
	myClient=(client_t*)arg;
	makeConnection(myClient);
	logClient(myClient);
	start(myClient);
}

void logClient(client_t* myClient)
{
	int aux, loged=0, msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	while(!loged)
	{
		if(rcvMsg(myClient->readFD,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		printf("msg: %d\n", msg);
		if(rcvString(myClient->readFD, name)<=0)
		{
			makeDisconnection(myClient);
		}
		printf("name: %s\n", name);
		if(rcvString(myClient->readFD, password)<=0)
		{
			makeDisconnection(myClient);
		}
		printf("psswd: %s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password, myClient))==0)
			{
				loged=1;
			}
			sndMsg(myClient->writeFD, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			printf("Hago el signup\n");
			aux=signUp(name, password);
			sndMsg(myClient->writeFD, (void*)&aux, sizeof(int));
			if(aux==0)
			{
			    loged=1;
			    printf("voy al login\n");
			    logIn(name, password, myClient);
			}
		}
	}
}

void start(client_t* myClient)
{
	int msg, lID, tID;
	while(1)
	{
		printf("recibiendo comando...");
		if(rcvMsg(myClient->readFD,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		printf("%d\n",msg);
		switch(msg)
		{
			case SEND_LEAGUE:
				listLeagues(myClient->writeFD);
				break;
			case SEND_TEAM:
				listTeam(myClient->user ,myClient->writeFD);
				break;
			case SEND_TRADE:
				listTrades(myClient->user, myClient->writeFD);
				break; 
			case LEAGUE_SHOW:
				rcvMsg(myClient->readFD, (void*)&msg, sizeof(int));
				if(msg<lCant && msg>=0)
				{
					leagueShow(leagues[msg], myClient->writeFD, LEAGUE_SHOW, END_LEAGUE_SHOW);
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
				}
				break;
			case TEAM_SHOW:
				rcvMsg(myClient->readFD, (void*)&msg, sizeof(int));
				lID=msg/CONVERSION;
				tID=msg%CONVERSION;
				if(lID<lCant && lID>=0)
				{
					if(tID<leagues[lID]->tCant && tID>=0)
					{
						teamShow(leagues[lID]->teams[tID], myClient->writeFD, TEAM_SHOW, END_TEAM_SHOW);
					}
					else
					{
						msg=ID_INVALID;
						sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
				}
				break;
			case TRADE_SHOW:
				rcvMsg(myClient->readFD, (void*)&msg, sizeof(int));
				lID=msg/CONVERSION;
				tID=msg%CONVERSION;
 				if(lID<lCant && lID>=0)
				{
					trade_t* tradeAux = getTradeByID(leagues[lID], tID);
					if(tradeAux!=NULL)
					{
						tradeShow(tradeAux, myClient->writeFD);
					}
					else
					{
						msg=ID_INVALID;
						sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
				}
				break;
			case DRAFT:
				rcvMsg(myClient->readFD, (void*)&msg, sizeof(int));
				if(msg>=0 && msg<lCant && leagues[msg]->draft!=NULL)
				{
					team_t* team= getTeamByClient(leagues[msg], myClient);
					if(team==NULL)
					{
						msg=ID_INVALID;
						sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
					}
					else
					{
						leagues[msg]->draft->clients[team->ID]=myClient;
						controlDraft(leagues[msg]->draft);
						msg=DRAFT_WAIT;
						sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->writeFD, (void*)&msg, sizeof(int));
				}
				break;
		}
	}
}

void makeDisconnection(client_t* myClient)
{
	disconnect(myClient->readFD);
	disconnect(myClient->writeFD);
	printf("cliente desconectado\n");
	pthread_exit(0);
}

void makeConnection(client_t* myClient)
{
	char readChannel[4], writeChannel[4];
	int id=myClient->ID;
	sprintf(writeChannel, "%c%d", 's', id);
	create(writeChannel);
	myClient->writeFD=connectChannel(writeChannel, O_WRONLY);
	sprintf(readChannel, "%c%d", 'c', id);
	create(readChannel);
	myClient->readFD=connectChannel(readChannel, O_RDONLY);

}

void controlDraft(draft_t* draft)
{
	int i;
	for(i=0; i< (draft->league->tMax) ;i++)
	{
		if(!draft->clients[i])
		{
			return;
		}
	}
	pthread_t draftT;
	pthread_create(&draftT, NULL, draftAttendant, (void*) draft);
}

