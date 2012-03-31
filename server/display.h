#ifndef __SERVER__
#define __SERVER__
#include "league.h"
#include "list.h"
#include "externvars.h"
#include "arrays.h"
#include "cmp.h"
#include "../msg.h"
#include <stdio.h>

#define NO_TEAM -1

void listLeagues(void* writeChannel);
void sendTrade(trade_t* trade, void* writeChannel);
int involved(trade_t* trade, user_t* user);
void listTrades(user_t* user, void* writeChannel);
void listTeam(user_t* user, void* writeChannel);
void createOrderedList(listADT list, league_t* league);
void sendTeam(team_t* team, void* writeChannel, int code);
void sendTeams(listADT teams, void* writeChannel, int code);
void leagueShow(league_t* league, void* writeChannel, int code, int end);
void sendSportists(league_t* league, int teamID, void* writeChannel, int code);
void teamShow(team_t* team, void* writeChannel, int code, int end);
void tradeShow(trade_t* trade, void* writeChannel);
void sendSporist(league_t* league, int spID, void* channel, int code);
void sendAllSportists(league_t* league, void* channel, int code);

#endif
