#ifndef __SERVER__
#define __SERVER__
#include "league.h"
#include "list.h"
#include "cmp.h"
#include <stdio.h>
#define INCORRECT_PASSWORD 1
#define USER_NOT_FOUND 2
#define NO_TEAM -1
#define LOGIN 1
#define SIGNUP 2
#define USER_NAME_OCCUPIED 3
#define NAME_OR_PASSWORD_TOO_LARGE 4
#define DEFAULTID 0
#define NEWCLIENT 5

listADT leagues;
listADT users;
listADT clients;
listADT connected;
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

typedef struct{
	int ID;
	user_t* user;
}client_t;


#endif
