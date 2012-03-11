#include "list.h"

typedef struct
{
    int ID;
    listADT teams;
    char name[10];
    char password[10];
}user_t;
    

typedef struct
{
    char name[15];
    listADT sportists;
    listADT teams;
    listADT trades;
}league_t;


typedef struct
{
    char name[20];
    league_t* league;
    int points;
    listADT sportists;
}team_t;
    
    
typedef char sportist_t[30];
    
typedef struct 
{
    team_t* from;
    team_t* to;
    sportist_t* offer;
    sportist_t* change;
}trade_t;

