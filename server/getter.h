#ifndef __GETTER__
#define __GETTER__

#include "list.h"
#include "league.h"
#include "externvars.h"


sportist_t* getSportistByID(league_t* league, int sportistID);

team_t* getTeamByID(league_t* league, int teamID);

user_t* getUserByID(int userID);

league_t* getLeagueByID(int leagueID);

team_t* getTeamByClient(league_t* league, client_t* client);

trade_t* getTradeByID(league_t* league, int searchID);

client_t* getClientByID(int id);

#endif
