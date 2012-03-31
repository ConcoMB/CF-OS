#ifndef __FILES__
#define __FILES__

#include <stdio.h>
#include <stdlib.h>
#include "league.h"
#include "list.h"
#include "getter.h"
#include "externvars.h"
#include "cmp.h"
#include "arrays.h"

void saveAll();

void saveUsers();

void saveUser(FILE* userFile, user_t* user);
void saveLeagues();
void saveLeague(FILE* leagueFile, league_t* league);
void saveTeam(FILE* teamFile, team_t* team);
void saveTrade(FILE* tradeFile, trade_t* trade);
void saveSportist(FILE* sportistFile, sportist_t* sportist);

/* LOAD */

void loadAll();
void loadUsers();
user_t* loadUser(FILE* userFile);
void loadLeagues();
league_t* loadLeague(FILE* leagueFile);
void loadNewSportists(league_t* league);
void loadTeams(league_t* league);
team_t* loadTeam(FILE* teamFile, league_t* league);
void loadTrades(league_t* league);
trade_t* loadTrade(FILE* tradeFile, league_t* league);
void loadSportists(league_t* league);
sportist_t* loadSportist(FILE* sportistFile, league_t* league);

#endif

//HACER UN LOADALL
