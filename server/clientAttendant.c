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
#include "trade.h"
#include <sys/shm.h>
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
	myClient=(client_t*)arg;
	makeConnection(myClient);
	while(1)
	{
		logClient(myClient);
	}
}

void logClient(client_t* myClient)
{
	int aux, loged=0, msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	while(!loged)
	{
		if(rcvMsg(myClient->channel,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		printf("msg: %d\n", msg);
		if(rcvString(myClient->channel, name)<=0)
		{
			makeDisconnection(myClient);
		}
		printf("name: %s\n", name);
		if(rcvString(myClient->channel, password)<=0)
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
			sndMsg(myClient->channel, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			aux=signUp(name, password);
			sndMsg(myClient->channel, (void*)&aux, sizeof(int));
			if(aux==0)
			{
			    loged=1;
			    logIn(name, password, myClient);
			}
		}
	}
	start(myClient);
}

void start(client_t* myClient)
{
	int msg, lID, tID, offer, change;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	team_t * team;
	trade_t* trade;
	while(1)
	{
		sleep(2);
		if(rcvMsg(myClient->channel,(void*)&msg, sizeof(int))<=0)
		{
			makeDisconnection(myClient);
		}
		switch(msg)
		{
			case SEND_LEAGUE:
				listLeagues(myClient->channel);
				break;
			case SEND_TEAM:
				listTeam(myClient->user ,myClient->channel);
				break;
			case SEND_TRADE:
				listTrades(myClient->user, myClient->channel);
				break; 
			case LEAGUE_SHOW:
				rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
				if(msg<lCant && msg>=0)
				{
					leagueShow(leagues[msg], myClient->channel, LEAGUE_SHOW, END_LEAGUE_SHOW);
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case TEAM_SHOW:
				rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
				lID=msg/CONVERSION;
				tID=msg%CONVERSION;
				if(lID<lCant && lID>=0)
				{
					if(tID<leagues[lID]->tCant && tID>=0)
					{
						teamShow(leagues[lID]->teams[tID], myClient->channel, TEAM_SHOW, END_TEAM_SHOW);
					}
					else
					{
						msg=ID_INVALID;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case TRADE_SHOW:
				rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
				lID=msg/CONVERSION;
				tID=msg%CONVERSION;
 				if(lID<lCant && lID>=0)
				{
					trade= getTradeByID(leagues[lID], tID);
					if(trade!=NULL)
					{
						tradeShow(trade, myClient->channel);
					}
					else
					{
						msg=ID_INVALID;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case DRAFT:
				rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
				if(msg>=0 && msg<lCant && leagues[msg]->draft!=NULL)
				{
					team_t* team= getTeamByClient(leagues[msg], myClient);
					if(team==NULL)
					{
						msg=ID_INVALID;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
					else
					{
						leagues[msg]->draft->clients[leagues[msg]->draft->turn++]=myClient;
						controlDraft(leagues[msg]->draft);
						msg=DRAFT_WAIT;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case MAKE_TRADE:
				rcvMsg(myClient->channel, (void*)&lID, sizeof(int));
				rcvMsg(myClient->channel, (void*)&offer, sizeof(int));
				rcvMsg(myClient->channel, (void*)&change, sizeof(int));
				if(lID!=-1)
				{
					tID=lID%CONVERSION;
					lID/=CONVERSION;

				}
				else
				{
					lID=offer/CONVERSION;
					tID=-1;
				}
				offer%=CONVERSION;
				change%=CONVERSION;
				if(lID==offer/CONVERSION && lID==change/CONVERSION && lID<lCant && lID>=0 && 
					(tID==-1 || (tID<leagues[lID]->tCant && tID>=0)) && 
					(team=getTeamByClient(leagues[lID], myClient))!=NULL && offer>=0 && change>=0 &&
					offer<CANT_SPORTIST && change<CANT_SPORTIST && (tID==-1 ||
					team->ID!=leagues[lID]->teams[tID]->ID))
				{
					if(tID!=-1)
					{
						if(offerTrade(leagues[lID], team, leagues[lID]->teams[tID], leagues[lID]->sportists[offer], leagues[lID]->sportists[change])==0)
						{
							msg=TRADE_OFFERED;
						}
						else
						{
							msg=ERROR;
						}
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
					else
					{
						if(makeTrade(leagues[lID], team, leagues[lID]->sportists[offer], leagues[lID]->sportists[change])==0)
						{
							msg=TRADE_MADE;
						}
						else
						{
							msg=ERROR;
						}
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}

				break;
			case TRADE_WD:
				rcvMsg(myClient->channel, (void*)&tID, sizeof(int));
				lID=tID/CONVERSION;
				tID%=CONVERSION;
				if(lID<lCant && lID>=0 && (team=getTeamByClient(leagues[lID], myClient))!=NULL)
				{
					if(withdrawTrade(team, tID, leagues[lID])==0)
					{
						msg=TRADE_WD;
					}
					else
					{
						msg=ERROR;
					}
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				else
				{
					msg= ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case TRADE_YES:
				rcvMsg(myClient->channel, (void*)&tID, sizeof(int));
				lID=tID/CONVERSION;
				tID%=CONVERSION;
				if(lID<lCant && lID>=0 && (trade=getTradeByID(leagues[lID], tID))!=NULL)
				{
					acceptTrade(trade, leagues[lID]);
					msg=TRADE_YES;
				}
				else
				{
					msg= ID_INVALID;
				}
				sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				break;
			case TRADE_NEG:
				rcvMsg(myClient->channel, (void*)&tID, sizeof(int));
				rcvMsg(myClient->channel, (void*)&offer, sizeof(int));
				rcvMsg(myClient->channel, (void*)&change, sizeof(int));
				lID=tID/CONVERSION;
				tID%=CONVERSION;
				trade=getTradeByID(leagues[lID], tID);
				if(lID==offer/CONVERSION && lID== change/CONVERSION &&
					lID<lCant && lID>=0 && trade!=NULL && offer>=0 &&
					change>=0 && offer<CANT_SPORTIST && change<CANT_SPORTIST && 
					trade->to->user->ID==myClient->user->ID)
				{
					offer%=CONVERSION;
					change&=CONVERSION;
					if(negociate(trade, leagues[lID]->sportists[offer], leagues[lID]->sportists[change], leagues[lID])==0)
					{
						msg=TRADE_NEG;
					}
					else
					{
						msg=ERROR;
					}
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));

				}
				else
				{
					msg= ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case MAKE_LEAGUE:
				
				rcvString(myClient->channel, name);
				rcvString(myClient->channel, password);
				rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
				msg=createLeague(name, password, msg);
				if(msg==0)
				{
					msg=MAKE_LEAGUE;
				}
				sndMsg(myClient->channel, (void*)&msg, sizeof(int));

				break;
			case JOIN_LEAGUE:
				rcvMsg(myClient->channel, (void*)&lID, sizeof(int));
				if(lID<lCant && lID>=0)
				{
					if(userAlreadyJoined(leagues[lID], myClient->user))
					{
						msg=USER_ALREADY_JOINED;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
					}
					else
					{

						msg=JOIN_LEAGUE;
						sndMsg(myClient->channel, (void*)&msg, sizeof(int));
						rcvString(myClient->channel, name);
						if(teamNameOccupied(leagues[lID], name))
						{
							msg=NAME_TAKEN;
							sndMsg(myClient->channel, (void*)&msg, sizeof(int));
						}
						else
						{
							msg=JOIN_LEAGUE;
							sndMsg(myClient->channel, (void*)&msg, sizeof(int));
							if(leagues[lID]->password[0] == '\0')
							{
								msg=NO_PASSWORD;
								sndMsg(myClient->channel, (void*)&msg, sizeof(int));
								password[0]='\0';
							}
							else
							{
								msg=SEND_PASSWORD;
								sndMsg(myClient->channel, (void*)&msg, sizeof(int));
								rcvString(myClient->channel, password);
							}
							msg = joinLeague(myClient->user, leagues[lID], name, password);
							if(msg==0)
							{
								msg=JOIN_LEAGUE;
							}
							sndMsg(myClient->channel, (void*)&msg, sizeof(int));
						}
					}
				}
				else
				{
					msg=ID_INVALID;
					sndMsg(myClient->channel, (void*)&msg, sizeof(int));
				}
				break;
			case LOG_OUT:
				return;
		}
	}
}

void makeDisconnection(client_t* myClient)
{
	disconnect(myClient->channel);
	printf("cliente desconectado\n");
	pthread_exit(0);
}

void makeConnection(client_t* myClient)
{
	int id=myClient->ID;
	//createChannel(id);
	//createChannel(id+1);
	myClient->channel=connectChannel(id);
	//myClient->channel=connectChannel(id+1);

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

