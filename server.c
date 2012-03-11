#include "league.h"
#include "list.h"
#include "cmp.h"
#include <stdio.h>

void list_leagues(user_t* user)
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

void printTrade(trade_t* trade)
{
    printf("The team %s has offered the team %s to exchange his %s to %s\n", trade->from->name, trade->to->name, (char*)trade->offer, (char*)trade->change);
}
void list_trades(user_t* user)
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
    

int involved(trade_t* trade, user_t* user)
{
    if(trade->from->user->ID==user->ID || trade->to->user->ID==user->ID)
    {
        return 1;
    }
    return 0;
}

                

void list_team(user_t* user)
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


void createOrderedList(listADT list, league_t* league)
{
    team_t* team;
    reset(league->teams);
    while((team=(team_t*)getNext(league->teams))!=NULL)
    {
        insert(list, team);
    }
    return;
}

void displayLeague(listADT teams)
{
    team_t* team;
    reset(teams);
    while((team=(team_t*)getNext(teams))!=NULL)
    {
        printf("Team %s, from user %s  ->  %d points\n", team->name, team->user->name, team->points);
    }
}

void leagueShow(league_t* league)
{
    listADT list = newList(cmpTeam);
    createOrderedList(list, league);
    displayLeague(list);
}
        
