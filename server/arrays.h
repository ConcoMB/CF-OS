#include "externvars.h"
#include "league.h"
#include <stdlib.h>
#define ARRAY_SIZE 20

void resizeUser();

void resizeLeagues();

void resizeTeams();

void resizeSportist();

void newUser(user_t* user);

void newLeague(league_t* league);

void newTeam(league_t * league, team_t * team);

void newSportist(sportist_t * sportist);
