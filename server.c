#include "server.h"

void listLeagues(user_t* user)
{
    if(user->teams!=NULL)
    {
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            printf("Participating in: %s\n", team->league->name);
        }
    }
}

void printTrade(trade_t* trade)
{
    printf("The team %s has offered the team %s to exchange his %s to %s\n", 
    trade->from->name, trade->to->name, (char*)trade->offer, (char*)trade->change);
}

int involved(trade_t* trade, user_t* user)
{
    if(trade->from->user->ID==user->ID || trade->to->user->ID==user->ID)
    {
        return 1;
    }
    return 0;
}

void listTrades(user_t* user)
{
    if(user->teams!=NULL)
    {   
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            league_t* league=team->league;
            if(league->trades!=NULL)
            {
                trade_t* trade;
                reset(league->trades);
                while((trade=(trade_t*)getNext(league->trades))!=NULL)
                {
                    if(involved(trade, user))
                    {
                        printTrade(trade);
                    }
                }
            }
        }
    }
}
    

void listTeam(user_t* user)
{
    if(user->teams!=NULL)
    {
        team_t* team;
        reset(user->teams);
        while((team=(team_t*)getNext(user->teams))!=NULL)
        {
            printf("Leader of the team %s\n", team->name);
        }
    }
}


void createOrderedList(listADT list, league_t* league)
{
    team_t* team;
    reset(league->teams);
    while((team=(team_t*)getNext(league->teams))!=NULL)
    {
        insert(list, team);
    }
    return;
}

void displayTeam(team_t* team)
{
    printf("Team %s, from user %s  ->  %d points\n", team->name, team->user->name, team->points);
}
void displayTeams(listADT teams)
{
    team_t* team;
    reset(teams);
    while((team=(team_t*)getNext(teams))!=NULL)
    {
        displayTeam(team);
    }
}

void leagueShow(league_t* league)
{
    team_t * team;
    listADT list = newList(cmpTeam);
    createOrderedList(list, league);
    displayTeams(list);
    reset(league->teams);
    while((team=(team_t*)getNext(league->teams))!=NULL)
    {
        displaySportists(team->sportists);
    }
    displaySportists(sportists);

}

void displaySportist(listADT list)
{
    sportist_t* sp;
    reset(sp);
    while((sp=(sportist_t*)getNext(list))!=NULL)
    {
        printf("%s, %d points, %s\n", sp->name, sp-> score, sp->team->name);
    }
}

void teamShow(team_t* team)
{
    displayTeam(team);
    displaySportists(team->sportists, team->team->name);
}

void tradeShow(trade_t* trade)
{
    printf("The sportist %s from %s team has been offered in exchange of %s from %s team\n", 
            trade->offer->name, trade->from->name, trade->change->name, trade->to->name);
}

void offerTrade(league_t* league, team_t* from, team_t* to, sportist_t* offer, sportist_t* change)
{
    if(elementBelongs(from->sportists, (void*)offer) && elementBelongs(to->sportists, (void*)change))
    {
        //GENERAR EL TRADE(mandar msjes y tal)
        trade_t* trade = malloc(sizeof(trade_t*));
        if(trade==NULL){
            return 1;
        }
        trade->offer=offer;
        trade->to=to;
        trade->from=from;
        trade->change=change;
        insert(league->trades, trade);
    }
}

void withdrawTrade(trade_t* trade, league_t* league)
{
    delete(league->trades, trade);
    //mensajes
}

void spCopy(sporitst_t* target, sportist_t* source)
{
    target=malloc(sizeof(sportist_t*));
    if(target==NULL){
        //MUERE
        return;
    }
    target->ID=source->ID;
    strcpy(target->name, source->name);
    target->score=source->score;
}

void acceptTrade(trade_t* trade, league_t* league)
{
    sportist_t* sp1, sp2;
    spCopy(sp1, trade->offer);
    spCopy(sp2,trade->change);
    delete(trade->from->sportists, trade->offer);
    delete(trade->to->sportists, trade->change);
    insert(trade->from->sportists, sp2);
    insert(trade->to->sportists, sp1);
}

void negociate(trade_t* oldTrade, sportist_t* newOffer, sportist_t* newChange, league_t* league)
{
    withdrawTrade(oldTrade, league);
    offerTrade(league, oldTrade->to, oldTrade->from, newOffer, newChange);
}

int userAlreadyJoined(league_t* league, user_t* user)
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

int joinLeague(user_t* user, league_t* league, char* password, char* teamName)
{
    if(strcmp(password, league->password)!=0 || userAlreadyJoined(league, user))
    {
	   return 1;
    }
    team_t * newTeam = malloc(sizeof(team_t*));
    newTeam->user=user;
    newTeam->name=teamName;
    newTeam->points=0;
    newTeam->sportists=newList(cmpTeam);
    newTeam->ID=league->nextTeamID++;
    insert(league->teams, newTeam);
}

int leagueNameOccupied(char* name)
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

int createLeague(char* name, char* password)
{
    if(leagueNameOccupied(name))
    {
        return 1;
    }
    league_t newLeague=malloc(sizeof(league_t*));
    newLeague->ID;
    newLeague->nextTeamID=0;
    strcpy(newLeague->name, name);
    strcpy(newLeague->password,password);
    newLeague->sportists=newList(cmpSportists);
    newLeague->teams=newList(cmpTeam);
    newList->trades=newList(cmpTrades);
    return 0;
}

int userNameOccupied(char* name)
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
        return 1;
    }
    user_t* newUser=malloc(sizeof(user_t*));
    newUser->ID=nextUserID++;
    newUser->teams=newList(cmpTeam);
    strcpy(newUser->name, name)
    strcpy(newUser->password, password)
    //falta validar la longitud de los strringsss
    return 0;
}

sportist_t getSportistByID(league_t league, int sportistID){
    sportist_t sportist;
    team_t team;
    restart(league->sportists);
    while(sportist=getNext(league->sportists)){
      if(sportist->ID==sportistID){
	       return sportist;
      }
    }
    restart(league->teams);
    while(team=getNext(league->teams)){
      restart(team->sportists);
      while(sportist=getNext(team->sportists)){
	       if(sportist->ID==sportistID){
	           return sportist;
	       }
      }
    }
}

team_t getTeamByID(league_t league, int teamID){
    team_t team;
    restart(league->teams);
    while(team=getNext(league->teams)){
      if(team->ID==teamID){
	return team;
      }
    }
}

user_t getUserByID(int userID){
    user_t user;
    restart(users);
    while(user=getNext(users)){
      if(user->ID==userID){
	return user;
      }
    }
}

league_t getLeagueByID(int leagueID){
    league_t league;
    restart(leagues);
    while(league=getNext(leagues)){
      if(league->ID==leagueID){
	return league;
      }
    }
}
