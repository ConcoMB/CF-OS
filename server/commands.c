#include "commands.h"
#include "externvars.h"
#include "getter.h"

void (*cmds[])(client_t*)=
{
	cmdSendLeague,
	cmdSendTeam,
	cmdSendTrade,
	cmdLeagueShow,
	cmdTeamShow,
	cmdTradeShow,
	cmdDraft,
	cmdMakeTrade,
	cmdTradeWD,
	cmdTradeYes,
	cmdTradeNeg,
	cmdMakeLeague,
	cmdJoinLeague,
	cmdLogout
};

void cmdSendLeague(client_t* myClient)
{
	listLeagues(myClient->channel);
}

void cmdSendTeam(client_t* myClient)
{
	listTeam(myClient->user ,myClient->channel);
}

void cmdSendTrade(client_t* myClient)
{
	listTrades(myClient->user, myClient->channel);
}

void cmdLeagueShow(client_t* myClient)
{
	int msg;
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
}

void cmdTeamShow(client_t* myClient)
{
	int msg, lID, tID;
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
}

void cmdTradeShow(client_t* myClient)
{
	trade_t* trade;
	int msg, lID, tID;
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
}

void cmdDraft(client_t* myClient)
{
	int msg;
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
}

void cmdMakeTrade(client_t* myClient)
{
	int lID, offer, change, tID, msg; 
	team_t* team;
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
}

void cmdTradeWD(client_t* myClient)
{
	int msg, lID, tID;
	team_t* team;
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
}

void cmdTradeYes(client_t* myClient)
{
	int msg, tID, lID;
	trade_t* trade;
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
}

void cmdTradeNeg(client_t* myClient)
{
	int tID, lID, offer, change, msg;
	trade_t* trade;
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
}

void cmdMakeLeague(client_t* myClient)
{
	int msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	rcvString(myClient->channel, name);
	rcvString(myClient->channel, password);
	rcvMsg(myClient->channel, (void*)&msg, sizeof(int));
	msg=createLeague(name, password, msg);
	if(msg==0)
	{
		msg=MAKE_LEAGUE;
	}
	sndMsg(myClient->channel, (void*)&msg, sizeof(int));
}

void cmdJoinLeague(client_t* myClient)
{
	char name[NAME_LENGTH], password[NAME_LENGTH];
	int msg, lID;
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
}

void cmdLogout(client_t* myClient)
{
	return;
}
