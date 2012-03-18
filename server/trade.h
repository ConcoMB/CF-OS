#ifndef __TRADE__
#define __TRADE__

#include "list.h"
#include "league.h"
#include <stdlib.h>

void offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change);

void withdrawTrade(trade_t* trade, league_t* league);

void acceptTrade(trade_t* trade, league_t* league);

void negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league);


#endif