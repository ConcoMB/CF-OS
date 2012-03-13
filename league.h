#ifndef __LEAGUE__
#define __LEAGUE__
#include "list.h"

#define NAME_LENGTH 15
#define SPORT_NAME_L 30

typedef struct
{
    int ID;
    listADT teams;
    char name[NAME_LENGTH];
    char password[NAME_LENGTH];
}user_t;
    

typedef struct
{
	int ID;
    int nextTeamID;
    char name[NAME_LENGTH];
    char password[NAME_LENGTH];
    listADT sportists; 
    listADT teams;
    listADT trades;
}league_t;


typedef struct
{
	int ID;
    char name[NAME_LENGTH];
    user_t* user; 
    league_t* league;
    int points;
}team_t;
    
typedef  struct
{
	int ID;
	char name[SPORT_NAME_L];
	int score;
	team_t* team;
}sportist_t;
    
typedef struct 
{
	int ID;
	league_t* league;
    team_t* from;
    team_t* to;
    sportist_t* offer;
    sportist_t* change;
}trade_t;

#endif
