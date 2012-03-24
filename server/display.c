#include "display.h"

void listLeagues(int writeChannel)
{
    printf("entre a listleagues\n");
    int i, msg=SEND_LEAGUE;
    for(i=0; i<lCant; i++)
    {
	   sndMsg(writeChannel,(void*)&msg, sizeof(int));
       printf("%s\n", leagues[i]->name);
	   sndString(writeChannel, leagues[i]->name);
	   sndMsg(writeChannel, (void*)&(leagues[i]->ID), sizeof(int));
    }
    msg=END_SEND_LEAGUE;
    sndMsg(writeChannel, (void*)&msg, sizeof(int));

}

static void sendTrade(trade_t* trade, int writeChannel)
{
    int code=SEND_TRADE;
    sndMsg(writeChannel, (void*)&code, sizeof(int));
    char msg[50];
    sprintf(msg, "In the league %s the team %s has offered the team %s to exchange his %s to %s\n", 
    trade->league->name, trade->from->name, trade->to->name, trade->offer->name, trade->change->name);
    sndString(writeChannel, msg);
}

static int involved(trade_t* trade, user_t* user)
{
    if(trade->from->user->ID==user->ID || trade->to->user->ID==user->ID)
    {
        return 1;
    }
    return 0;
}

void listTrades(user_t* user, int writeChannel)
{
    if(user->teams!=NULL)
    {   
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            league_t* league=team->league;
            if(league->trades!=NULL)
            {
                trade_t* trade;
                reset(league->trades);
                while((trade=(trade_t*)getNext(league->trades))!=NULL)
                {
                    if(involved(trade, user))
                    {
                        sendTrade(trade, writeChannel);
                    }
                }
            }
        }
    }
    int msg=END_SEND_TRADE;
    sndMsg(writeChannel, (void*)&msg, sizeof(int));
}
    

void listTeam(user_t* user, int writeChannel)
{
    printf("entre al display\n");
    int msg;
    if(user->teams!=NULL)
    {
        team_t* team;
        reset(user->teams);
        printf("pase el reset\n");
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
		  msg=SEND_TEAM;
		  sndMsg(writeChannel, (void*)&msg, sizeof(int));
		  sndString(writeChannel, team->name);
		  sndString(writeChannel, team->league->name);
		  msg=team->points;
		  sndMsg(writeChannel, (void*)&msg, sizeof(int));
	   }
	msg=END_SEND_TEAM;
	sndMsg(writeChannel, (void*)&msg, sizeof(int));
    printf("termine\n");
    }
}


static void createOrderedList(listADT list, league_t* league)
{
    int i;
    for( i =0; i<league->tCant; i++)
    {
        insert(list, league->teams[i]);
    }
    return;
}

static void sendTeam(team_t* team, int writeChannel)
{
	char msg[50];
    sprintf(msg, "Team %s, from user %s  ->  %d points\n", team->name, team->user->name, team->points);
    int code=LEAGUE_SHOW;
    sndMsg(writeChannel, (void*)&code, sizeof(int));
    sndString(writeChannel, msg);
}
static void sendTeams(listADT teams, int writeChannel)
{
    team_t* team;
    reset(teams);
    while((team=(team_t*)getNext(teams))!=NULL)
    {
        sendTeam(team, writeChannel);
    }
}

void leagueShow(league_t* league, int writeChannel)
{
    team_t * team;
    listADT list = newList(cmpTeam);
    createOrderedList(list, league);
    sendTeams(list, writeChannel);
    sendSportists(league->sportists, NO_TEAM, writeChannel);
    int end= END_LEAGUE_SHOW;
    sndMsg(writeChannel, (void*)&end, sizeof(int));
    freeList(list);
}

static void sendSportists(sportist_t* sportists[], int teamID, int writeChannel)
{
    int i;
    for( i=0; i<CANT_SPORTIST; i++)
    {
        if(teamID==NO_TEAM || sportists[i]->team->ID==teamID){
          char msg[50];	  
	  sprintf(msg, "%s, %d points, %s\n", sportists[i]->name, sportists[i]-> score, sportists[i]->team->name);           
	    int code=LEAGUE_SHOW;
	sndMsg(writeChannel, (void*)&code, sizeof(int));
	sndString(writeChannel, msg);  
        }
    }
}
/*)
void teamShow(team_t* team)
{
    senTeam(team);
    sendSportists(team->league->sportists, team->ID);
}*/

void tradeShow(trade_t* trade)
{
    printf("The sportist %s from %s team has been offered in exchange of %s from %s team\n", 
            trade->offer->name, trade->from->name, trade->change->name, trade->to->name);
}


