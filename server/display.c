#include "display.h"

int nextClientID=1;
int nextUserID=0;
listADT leagues;
listADT users;
listADT clients;
listADT connected;

void listLeagues(user_t* user)
{
    if(user->teams!=NULL)
    {
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            printf("Participating in: %s\n", team->league->name);
        }
    }
}

static void printTrade(trade_t* trade)
{
    printf("The team %s has offered the team %s to exchange his %s to %s\n", 
    trade->from->name, trade->to->name, (char*)trade->offer, (char*)trade->change);
}

static int involved(trade_t* trade, user_t* user)
{
    if(trade->from->user->ID==user->ID || trade->to->user->ID==user->ID)
    {
        return 1;
    }
    return 0;
}

void listTrades(user_t* user)
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
                        printTrade(trade);
                    }
                }
            }
        }
    }
}
    

void listTeam(user_t* user)
{
    if(user->teams!=NULL)
    {
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            printf("Leader of the team %s\n", team->name);
        }
    }
}


static void createOrderedList(listADT list, league_t* league)
{
    team_t* team;
    reset(league->teams);
    while((team=(team_t*)getNext(league->teams))!=NULL)
    {
        insert(list, team);
    }
    return;
}

static void displayTeam(team_t* team)
{
    printf("Team %s, from user %s  ->  %d points\n", team->name, team->user->name, team->points);
}
static void displayTeams(listADT teams)
{
    team_t* team;
    reset(teams);
    while((team=(team_t*)getNext(teams))!=NULL)
    {
        displayTeam(team);
    }
}

void leagueShow(league_t* league)
{
    team_t * team;
    listADT list = newList(cmpTeam);
    createOrderedList(list, league);
    displayTeams(list);
    displaySportists(league->sportists, NO_TEAM);
    freeList(list);
}

static void displaySportists(listADT list, int teamID)
{
    sportist_t* sp;
    reset(list);
    while((sp=(sportist_t*)getNext(list))!=NULL)
    {
        if(teamID==NO_TEAM || sp->team->ID==teamID){
            printf("%s, %d points, %s\n", sp->name, sp-> score, sp->team->name);            
        }
    }
}

void teamShow(team_t* team)
{
    displayTeam(team);
    displaySportists(team->league->sportists, team->ID);
}

void tradeShow(trade_t* trade)
{
    printf("The sportist %s from %s team has been offered in exchange of %s from %s team\n", 
            trade->offer->name, trade->from->name, trade->change->name, trade->to->name);
}


