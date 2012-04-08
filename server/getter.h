#ifndef __GETTER__
#define __GETTER__

#include "list.h"
#include "league.h"
#include "externvars.h"

team_t* getTeamByClient(league_t* league, client_t* client);

trade_t* getTradeByID(league_t* league, int searchID);

client_t* getClientByID(int id);

#endif
