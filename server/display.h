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



void listLeagues(char* writeChannel);
void listTrades(user_t* user, char* writeChannel);
void listTeam(user_t* user, char* writeChannel);
void leagueShow(league_t* league);
void teamShow(team_t* team);
void tradeShow(trade_t* trade);


static void sendTrade(trade_t* trade, char* writeChannel);
static int involved(trade_t* trade, user_t* user);
static void createOrderedList(listADT list, league_t* league);
static void displayTeam(team_t* team);
static void displayTeams(listADT teams);
static void displaySportists(sportist_t* sportists[], int teamID);










#endif
