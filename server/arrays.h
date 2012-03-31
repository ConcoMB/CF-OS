#ifndef __ARRAYSS__
#define __ARRAYSS__

#include "externvars.h"
#include "league.h"
#include <stdlib.h>
#define ARRAY_SIZE 20

void resizeUsers();

void resizeLeagues();

void resizeTeams();

void resizeSportists();

void newUser(user_t* user);

void newLeague(league_t* league);

void newTeam(league_t * league, team_t * team);

void newSportist(league_t * league, sportist_t * sportist);

#endif