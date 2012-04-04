#include "join.h"


int teamNameOccupied(league_t* league, char name[NAME_LENGTH])
{
    int i;
    for(i=0; i<league->tCant; i++)
    {
        if(strcmp(name, league->teams[i]->name)==0)
        {
            return 1;
        }
    }
    return 0;
}

int userNameOccupied(char* name)
{
    int i;
    for( i=0; i<uCant; i++)
    {
        if(strcmp(name, users[i]->name)==0)
            return 1;
    }
    return 0;
}

int signUp(char* name, char* password)
{
    if(userNameOccupied(name))
    {
        return NAME_OCCUPIED;
    }
    if(strlen(name)>=NAME_LENGTH || strlen(password)>=NAME_LENGTH)
    {
        return NAME_OR_PASSWORD_TOO_LARGE;
    }
    user_t* user=malloc(sizeof(user_t));
    user->ID=uCant;
    user->draftLeague=-1;
    user->teams=newList(cmpTeam);
    strcpy(user->name, name);
    strcpy(user->password, password);
    newUser(user);
    return 0;
}

int logIn(char* name, char* password, client_t* client)
{
    int i;
    for( i =0; i<uCant; i++)
    {
        if(strcmp(users[i]->name, name)==0)
        {
            if(strcmp(users[i]->password, password)==0)
            {
                client->user=users[i];
                return 0;
            }
            else
            {
                return INCORRECT_PASSWORD;
            }
        }
    }
    return USER_NOT_FOUND;
}


int userAlreadyJoined(league_t* league, user_t* user)
{
    int i;
    for(i=0; i<league->tCant; i++) 
    {
        if(league->teams[i]->user->ID==user->ID)
        {
            return 1;
        }
    }
    return 0;
}

int joinLeague(user_t* user, league_t* league, char* teamName, char* password)
{
    if(league->tCant==league->tMax || league->draft==NULL)
    {
        return LEAGUE_FULL;
    }
    if(password[0]=='\0' || strcmp(password, league->password)==0)
    {
        team_t * team = malloc(sizeof(team_t));
        team->user=user;
        strcpy(team->name,teamName);
        team->points=0;
        team->ID=league->nextTeamID++;
        team->league=league;
        newTeam(league, team);
        insert(user->teams, team);
        return 0;
    }
    return INCORRECT_PASSWORD;
    
    
}


int leagueNameOccupied(char* name)
{
    int i;
    for(i =0; i<lCant; i++)
    {
        if(strcmp(name, leagues[i]->name)==0)
            return 1;
    }
    return 0;
}

int createLeague(char* name, char* password, int cant)
{
    int i;
    if(leagueNameOccupied(name))
    {
        return NAME_OCCUPIED;
    }
    league_t* league=malloc(sizeof(league_t));
    league->ID=lCant;
    league->nextTeamID=0;
    strcpy(league->name, name);
    strcpy(league->password, password);
    loadNewSportists(league);
    league->tCant=0;
    league->tMax=cant;
    league->nextTradeID=0;
    league->trades=newList(cmpTrade);
    draft_t* draft=malloc(sizeof(draft));
    draft->clients=malloc(sizeof(client_t*)*cant);
    draft->turn=0;
    draft->flag=-1;
    for(i=0; i<cant; i++)
    {
        draft->clients[i]=NULL;
    }
    league->draft=draft;
    newLeague(league);
    return 0;
}
