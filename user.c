#include "league.h"
#include <stdio.h>

void list_leagues(user_t* user)
{
    if(user->leagues!=NULL)
    {
        reset(user->leagues);
        while(user->leagues->current!=NULL)
        {
            printf("Participating in: %s\n", (char*)getNext(user->leagues));
        }
    }
}

void list_trades(user_t* user)
{
    if(user->leagues!=NULL)
    {
        reset(user->leagues);
        while(user->leagues->current!=NULL)
        {
            league_t league= getNext(user->leagues);
            if(league->trades!=NULL)
            {   
                reset(league->trades);
                while(league->trades->current!=NULL){
                
                    trade_t trade = getNext(league->trades);
                    if(involved(&trade, &user))
                    {
                        printTrade(&trade);
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

void printTrade(trade_t* trade)
{
    printf("The team %s has offered the team %s to exchange his %s to %s\n", trade->from->teamName, trade->to->teamName, (char*)trade->offer, (char*)trade->change);
}
                

void list_team(user_t* user)
{
    if(user->teams!=NULL)
    {
        reset(user->teams);
        while(user->teams->current!=NULL)
        {
            printTeam(getNext(user->teams));
        }
    }
}

void print
            
            
        