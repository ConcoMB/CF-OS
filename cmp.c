#include "cmp.h"

int cmpTeam(void* team1, void* team2)
{
    return ((team_t*)team1)->points-((team_t*)team2)->points;
}

int cmpSportist(void* sport1, void* sport2)
{
    return ((sportist_t*)sport1)->score - ((sportist_t*)sport2)->score;
}

int cmpTrade(void* trade1, void* trade2)
{
	return ((trade_t*)trade1)-> ID-((trade_t*)trade2)->ID;
}

int cmpUser(void* user1, void* user2)
{
	return ((user_t*)user1)->ID - ((user_t*)user2)->ID;
}

int cmpLeague(void* league1, void* league2)
{
	return ((league_t*)league1)->ID - ((league_t*)league2)->ID;
}
