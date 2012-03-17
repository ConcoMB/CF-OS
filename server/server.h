#ifndef __SERVER__
#define __SERVER__
#include "league.h"
#include "list.h"
#include "cmp.h"
#include "../msg.h"
#include <stdio.h>

#define NO_TEAM -1


extern listADT leagues;
extern listADT users;
extern listADT clients;
extern listADT connected;
extern int nextUserID;
extern int nextClientID;

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
int joinLeague(user_t* user, league_t* league, char* teamName);
int createLeague(char* name, char* password);
int signUp(char* name, char* password);
int logIn(char* name, char* password);
void displaySportists(listADT list, int teamID);




#endif
