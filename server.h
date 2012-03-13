#ifndef __SERVER__
#define __SERVER__
#include "league.h"
#include "list.h"
#include "cmp.h"
#include <stdio.h>

ListADT leagues;
ListADT users;
int nextUserID=0;

void listLeagues(user_t* user);
void listTrades(user_t* user);
void listTeam(user_t* user);
void leagueShow(league_t* league);
void teamShow(team_t* team);
void tradeShow(trade_t* trade);
void offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change);
void withdrawTrade(trade_t* trade, league_t* league);
void acceptTrade(trade_t* trade, league_t* league);
void negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league);



#endif
