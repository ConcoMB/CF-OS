#include "trade.h"


int offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change)
{
    if(offer->team->ID==from->ID && change->team->ID==to->ID && to->ID!=from->ID)
    {
        trade_t* trade = malloc(sizeof(trade_t));
        if(trade==NULL){
            return 1;
        }
        trade->offer=offer;
        trade->to=to;
        trade->from=from;
        trade->change=change;
        trade->ID=league->nextTradeID++;
        trade->league=league;
        insert(league->trades, trade);
        return 0;
    }
    else
        return 1;
}

int withdrawTrade(team_t* team, int tradeID, league_t* league)
{
    trade_t* trade;
    reset(league->trades);
    while((trade=getNext(league->trades))!=NULL)
    {
        if(trade->ID=tradeID)
        {
            if(trade->from->ID==team->ID)
            {
                delete(league->trades, trade);
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    return 1;
}


void acceptTrade(trade_t* trade, league_t* league)
{
    trade->offer->team=trade->to;
    trade->change->team=trade->from;  
    withdrawTrade(trade->from, trade->ID, league);    
}

int negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league)
{
    if(newOffer->team->ID!=oldTrade->to->ID || newChange->team->ID!=oldTrade->from->ID)
    {
        return 3;
    }

    if(withdrawTrade(oldTrade->from, oldTrade->ID, league))
    {
        return 2;
    }
    if(offerTrade(league, oldTrade->to, oldTrade->from, newOffer, newChange))
    {
        return 1;
    }
    return 0;
}
