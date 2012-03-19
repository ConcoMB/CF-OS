#include "joining.h"


static int userNameOccupied(char* name)
{
    user_t* user;
    reset(users);
    while((user=getNext(users))!=NULL)
    {
        if(strcmp(name, user->name)==0)
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
    user_t* newUser=malloc(sizeof(user_t*));
    newUser->ID=nextUserID++;
    newUser->teams=newList(cmpTeam);
    strcpy(newUser->name, name);
    strcpy(newUser->password, password);
    insert(users, newUser);
    return 0;
}

int logIn(char* name, char* password)
{
    user_t* user;
    reset(users);
    while((user=getNext(users))!=NULL)
    {
        if(strcmp(user->name, name)==0)
        {
            if(strcmp(user->password, password)==0)
            {
                insert(clients, user);
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


static int userAlreadyJoined(league_t* league, user_t* user)
{
    team_t* team;
    reset(league->teams);
    while((team=getNext(league->teams))!=NULL)
    {
        if(team->user->ID==user->ID)
            return 1;
    }
    return 0;
}

int joinLeague(user_t* user, league_t* league, char* teamName, char* password)
{
    if(strcmp(password, league->password)!=0)
    {
       return INCORRECT_PASSWORD;
    }
    if(userAlreadyJoined(league, user))
    {
        return USER_ALREADY_JOINED;
    }
    if(strlen(teamName)>=NAME_LENGTH)
    {
        return NAME_OR_PASSWORD_TOO_LARGE;
    }
    team_t * newTeam = malloc(sizeof(team_t));
    newTeam->user=user;
    strcpy(newTeam->name,teamName);
    newTeam->points=0;
    newTeam->ID=league->nextTeamID++;
    insert(league->teams, newTeam);
}


static int leagueNameOccupied(char* name)
{
    league_t* league;
    reset(leagues);
    while((league=getNext(leagues))!=NULL)
    {
        if(strcmp(name, league->name)==0)
            return 1;
    }
    return 0;
}

//REVISAR       
int createLeague(char* name, char* password)
{
    if(leagueNameOccupied(name))
    {
        return NAME_OCCUPIED;
    }
    if(strlen(name)>=NAME_LENGTH || strlen(password)>=NAME_LENGTH)
    {
        return NAME_OR_PASSWORD_TOO_LARGE;
    }
    league_t* newLeague=malloc(sizeof(league_t));
    newLeague->ID=nextLeagueID++;
    newLeague->nextTeamID=0;
    strcpy(newLeague->name, name);
    strcpy(newLeague->password,password);
    newLeague->sportists=newList(cmpSportist);
    newLeague->teams=newList(cmpTeam);
    newLeague->trades=newList(cmpTrade);
    return 0;
}
