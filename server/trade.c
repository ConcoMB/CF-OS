#include "trade.h"


void offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change)
{
    if(offer->team->ID==from->ID && change->team->ID==to->ID)
    {
        //GENERAR EL TRADE(mandar msjes y tal)
        trade_t* trade = malloc(sizeof(trade_t));
        if(trade==NULL){
            return 1;
        }
        trade->offer=offer;
        trade->to=to;
        trade->from=from;
        trade->change=change;
        insert(league->trades, trade);
    }
    //mala oferta
}

void withdrawTrade(trade_t* trade, league_t* league)
{
    delete(league->trades, trade);
    //mensajes
}


void acceptTrade(trade_t* trade, league_t* league)
{
    trade->offer->team=trade->to;
    trade->change->team=trade->from;      
}

void negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league)
{
    withdrawTrade(oldTrade, league);
    offerTrade(league, oldTrade->to, oldTrade->from, newOffer, newChange);
}
