#ifndef __LEAGUE__
#define __LEAGUE__
#include "list.h"
#include "../common.h"
#include <pthread.h>
#define SPORT_NAME_L 30
#define USERS_SHM 120

struct sportist_t;
struct team_t;

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
    char drafted;
    int nextTeamID;
    char name[NAME_LENGTH];
    char password[NAME_LENGTH];
    struct sportist_t** sportists; 
    struct team_t** teams;
    int tCant;
    listADT trades;
}league_t;

typedef struct{
    int ID;
    user_t* user;
    pthread_t att;
}client_t;

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
