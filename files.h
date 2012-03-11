#ifndef __FILES__
#define __FILES__

#include <stdio.h>
#include "league.h"
#include "server.h"

void saveUser(FILE* file,user_t* user);
void saveLeague(league_t* league);
void saveTeam(team_t* team);
void saveTrade(trade_t* trade);

void loadUsers(ListADT users);
static user_t* loadUser(FILE* userFile);
void loadLeagues(league_t* league);
static league_t* loadLeague(FILE* leagueFile);
void loadSportists(ListADT sportists);
void loadTeam(team_t* team);
void loadTrades();
static trade_t* loadTrade(FILE* tradeFile);
void loadScores(league_t league);

#endif
