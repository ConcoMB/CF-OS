#ifndef __FILES__
#define __FILES__

#include <stdio.h>
#include "league.h"

void saveUser(FILE* file,user_t* user);
void saveLeague(league_t* league);
void saveTeam(team_t* team);
void saveTrade(trade_t* trade);
void loadUser(user_t* user);
void loadLeague(league_t* league);

#endif
