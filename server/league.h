#ifndef __LEAGUE__
#define __LEAGUE__
#include "list.h"
#include "../common.h"
#include <pthread.h>
#define USERS_SHM 120
#define TEAM_SIZE 5

struct league_s;

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
    user_t* user;
    pthread_t att;
    pthread_t keepAliveThread;
    void* channel;
    time_t time;
}client_t;

    
typedef struct
{
    int ID;
    char name[NAME_LENGTH];
    user_t* user; 
    struct league_s* league;
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
    struct league_s* league;
    client_t ** clients;
    int flag;
    int turn;
}draft_t;

typedef struct league_s
{
	int ID;
    int nextTeamID;
    char name[NAME_LENGTH];
    char password[NAME_LENGTH];
    sportist_t* sportists[CANT_SPORTIST]; 
    team_t** teams;
    int tCant;
    int tMax;
    int nextTradeID;
    listADT trades;
    draft_t* draft;
}league_t;


    
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
