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

void listLeagues(int writeChannel);
static void sendTrade(trade_t* trade, int writeChannel);
static int involved(trade_t* trade, user_t* user);
void listTrades(user_t* user, int writeChannel);
void listTeam(user_t* user, int writeChannel);
static void createOrderedList(listADT list, league_t* league);
static void sendTeam(team_t* team, int writeChannel, int code);
static void sendTeams(listADT teams, int writeChannel, int code);
void leagueShow(league_t* league, int writeChannel, int code, int end);
static void sendSportists(sportist_t* sportists[], int teamID, int writeChannel, int code);
void teamShow(team_t* team, int writeChannel, int code, int end);
void tradeShow(trade_t* trade, int writeChannel);

#endif
