#ifndef __SERVER__
#define __SERVER__
#include "league.h"
#include "list.h"
#include "arrays.h"
#include "cmp.h"
#include "../msg.h"
#include <stdio.h>

#define NO_TEAM -1



void listLeagues(user_t* user);
void listTrades(user_t* user);
void listTeam(user_t* user);
void leagueShow(league_t* league);
void teamShow(team_t* team);
void tradeShow(trade_t* trade);


static void printTrade(trade_t* trade);
static int involved(trade_t* trade, user_t* user);
static void createOrderedList(listADT list, league_t* league);
static void displayTeam(team_t* team);
static void displayTeams(listADT teams);
static void displaySportists(listADT list, int teamID);










#endif
