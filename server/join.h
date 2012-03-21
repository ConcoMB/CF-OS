#ifndef __JOIN__
#define __JOIN__

#include "league.h"
#include "../msg.h"
#include <string.h>
#include "arrays.h"
#include "externvars.h"
#include "cmp.h"
#include "list.h"

int signUp(char* name, char* password);
int logIn(char* name, char* password, client_t* myClient);
int joinLeague(user_t* user, league_t* league, char* teamName, char* password);
int createLeague(char* name, char* password);

static int userAlreadyJoined(league_t* league, user_t* user);
static int userNameOccupied(char* name);
static int leagueNameOccupied(char* name);


#endif