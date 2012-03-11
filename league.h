
typedef struct user_t
{
    int ID;
    listADT teams;
    char[10] userName;
    char[10] password;
};
    
typedef struct team_t
{
    char[20] teamName;
    league_t * league;
    int points;
    listADT sportists;
};
    
typedef struct league_t
{
    listADT sportists;
    listADT teams;
    listADT trades;
};
    
typedef struct sportist_t char[30];
    
typedef struct trade_t
{
    leader_t from;
    leader_t to;
    sportist_t offer;
    sportist_t change;
}