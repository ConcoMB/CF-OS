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
static void sendTrade(trade_t* trade, void* writeChannel);
static int involved(trade_t* trade, user_t* user);
void listTrades(user_t* user, void* writeChannel);
void listTeam(user_t* user, void* writeChannel);
static void createOrderedList(listADT list, league_t* league);
static void sendTeam(team_t* team, void* writeChannel, int code);
static void sendTeams(listADT teams, void* writeChannel, int code);
void leagueShow(league_t* league, void* writeChannel, int code, int end);
static void sendSportists(league_t* league, sportist_t* sportists[], int teamID, void* writeChannel, int code);
void teamShow(team_t* team, void* writeChannel, int code, int end);
void tradeShow(trade_t* trade, void* writeChannel);

#endif
