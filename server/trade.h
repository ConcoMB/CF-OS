#ifndef __TRADE__
#define __TRADE__

#include "list.h"
#include "league.h"
#include <stdlib.h>

int offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change);

int withdrawTrade(team_t* team, int tradeID, league_t* league);

void acceptTrade(trade_t* trade, league_t* league);

int negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league);

int makeTrade(league_t* league, team_t* team, sportist_t* offer, sportist_t* change);

#endif