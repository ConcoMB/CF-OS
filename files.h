#ifndef __FILES__
#define __FILES__

#include <stdio.h>
#include "league.h"
#include "server.h"

void saveAll();
void saveUsers();
void saveLeagues();
void saveUser(FILE* file,user_t* user);
void saveLeague(File* leagueFile, league_t* league);
void saveTeam(FILE* teamFile, FILE* sportistFile, team_t* team);
void saveTrade(FILE* tradeFile, trade_t* trade);
void saveSportist(FILE* sportistFile, league_t league, sportist_t sportist, team_t team);

void loadUsers(ListADT users);
static user_t* loadUser(FILE* userFile);
void loadLeagues(league_t* league);
static league_t* loadLeague(FILE* leagueFile);
void loadNewSportists(ListADT sportists);
void loadTeam(team_t* team);
void loadTrades();
static trade_t* loadTrade(FILE* tradeFile);
void loadSportists(league_t league);

#endif

//HACER UN LOADALL