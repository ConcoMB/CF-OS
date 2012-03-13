#include "cmp.h"

int cmpTeam(void* team1, void* team2)
{
    return ((team_t*)team1)->points-((team_t*)team2)->points;
}

int cmpSportist(void* sport1, void* sport2)
{
    return strcmp((sportist_t*)sport1->name, (sportist_t*)sport2->name);
}

int cmpTrades(vpid* trade1, void* trade2)
{
	return ((trade_t*)trade1)-> ID-((trade_t*)trade2)->ID;
}