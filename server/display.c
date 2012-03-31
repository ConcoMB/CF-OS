#include "display.h"

void listLeagues(void* writeChannel) 
{
	int i, msg = SEND_LEAGUE;
	for (i = 0; i < lCant; i++)
	{
		sndMsg(writeChannel, (void*) &msg, sizeof(int));
		printf("%s\n", leagues[i]->name);
		sndString(writeChannel, leagues[i]->name);
		sndMsg(writeChannel, (void*) &(leagues[i]->ID), sizeof(int));
	}
	msg = END_SEND_LEAGUE;
	sndMsg(writeChannel, (void*) &msg, sizeof(int));
}

void sendTrade(trade_t* trade, void* writeChannel) 
{
	int code = SEND_TRADE;
	sndMsg(writeChannel, (void*) &code, sizeof(int));
	char string[150];
	sprintf(string,
			"In the league %s the team %s has offered the team %s to exchange his %s to %s (Trade ID %d)\n",
			trade->league->name, trade->from->name, trade->to->name,
			trade->offer->name, trade->change->name, trade->league->ID
					* CONVERSION + trade->ID);
	printf("%s\n", string);
	sndString(writeChannel, string);
}

int involved(trade_t* trade, user_t* user) 
{
	if (trade->from->user->ID == user->ID || trade->to->user->ID == user->ID) 
	{
		return 1;
	}
	return 0;
}

void listTrades(user_t* user, void* writeChannel) 
{
	if (user->teams != NULL) 
	{
		team_t* team;
		reset(user->teams);
		while ((team = (team_t*) getNext(user->teams)) != NULL) 
		{
			league_t* league = team->league;
			if (league->trades != NULL) 
			{
				trade_t* trade;
				reset(league->trades);
				while ((trade = (trade_t*) getNext(league->trades)) != NULL) 
				{
					if (involved(trade, user)) {
						sendTrade(trade, writeChannel);
					}
				}
			}
		}
	}
	int msg = END_SEND_TRADE;
	sndMsg(writeChannel, (void*) &msg, sizeof(int));
}

void listTeam(user_t* user, void* writeChannel) 
{
	int msg;
	if (user->teams != NULL) {
		team_t* team;
		reset(user->teams);
		while ((team = (team_t*) getNext(user->teams)) != NULL) 
		{
			int teamID = team->league->ID * CONVERSION + team->ID;
			msg = SEND_TEAM;
			sndMsg(writeChannel, (void*) &msg, sizeof(int));
			sndString(writeChannel, team->name);
			sndString(writeChannel, team->league->name);
			sndMsg(writeChannel, (void*) &teamID, sizeof(int));
			sndMsg(writeChannel, (void*) &(team->points), sizeof(int));
		}
		msg = END_SEND_TEAM;
		sndMsg(writeChannel, (void*) &msg, sizeof(int));
	}
}

void createOrderedList(listADT list, league_t* league) 
{
	int i;
	for (i = 0; i < league->tCant; i++) {
		insert(list, league->teams[i]);
	}
	return;
}

void sendTeam(team_t* team, void* writeChannel, int code) 
{
	char msg[100];
	sprintf(msg, "Team %s, ID %d , from user %s  ->  %d points\n", team->name,
			team->league->ID * CONVERSION + team->ID, team->user->name,
			team->points);
	sndMsg(writeChannel, (void*) &code, sizeof(int));
	sndString(writeChannel, msg);
}
void sendTeams(listADT teams, void* writeChannel, int code) 
{
	team_t* team;
	reset(teams);
	while ((team = (team_t*) getNext(teams)) != NULL) 
	{
		sendTeam(team, writeChannel, code);
	}
}

void leagueShow(league_t* league, void* writeChannel, int code, int end) 
{
	listADT list = newList(cmpTeam);
	createOrderedList(list, league);
	sendTeams(list, writeChannel, code);
	sendSportists(league, NO_TEAM, writeChannel, code);
	sndMsg(writeChannel, (void*) &end, sizeof(int));
	freeList(list);
}

void sendSportists(league_t* league, int teamID, void* writeChannel, int code) 
{
	int i;
	for (i = 0; i < CANT_SPORTIST; i++) 
	{
		if (teamID == NO_TEAM || league->sportists[i]->team->ID == teamID) 
		{
			
			sendSporist(league, i, writeChannel, code);
		}
		
	}
}

void sendSporist(league_t* league, int spID, void* channel, int code)
{
	char teamName[NAME_LENGTH];
	if(league->sportists[spID]->team==NULL)
	{
		strcpy(teamName, "no");
	}
	else
	{
		strcpy(teamName, league->sportists[spID]->team->name);
	}
	char string[200];
	sprintf(string, "%s, %d points, ID %d, %s team\n", league->sportists[spID]->name,
		league->sportists[spID]-> score, league->sportists[spID]->ID + CONVERSION* league->ID, teamName);
	sndMsg(channel, (void*) &code, sizeof(int));
	sndString(channel, string);
}

void teamShow(team_t* team, void* writeChannel, int code, int end) 
{
	sendTeam(team, writeChannel, code);
	sendSportists(team->league, team->ID, writeChannel, code);
	sndMsg(writeChannel, (void*) &end, sizeof(int));
}

void tradeShow(trade_t* trade, void* writeChannel) 
{
	char string[150];
	sprintf(string,
			"The sportist %s from %s team has been offered in exchange of %s from %s team (TRADE ID %d)\n",
			trade->offer->name, trade->from->name, trade->change->name,
			trade->to->name, trade->league->ID * CONVERSION + trade->ID);
	sndString(writeChannel, string);
}

void sendAllSportists(league_t* league, void* channel, int code)
{
	int i;
	for(i=0; i<CANT_SPORTIST; i++)
	{
		sendSporist(league, i, channel, code);
	}
}
