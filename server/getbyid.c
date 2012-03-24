#include "getbyid.h"

trade_t* getTradeByID(league_t* league, int searchID)
{
  trade_t* trade;
  reset(league->trades);
  while((trade=getNext(league->trades))!=NULL)
  {
    if(trade->ID==searchID)
    {
      return trade;
    }
  }
  return NULL;
}

/*
sportist_t* getSportistByID(league_t* league, int sportistID)
{
    sportist_t* sportist;
    team_t team;
    reset(league->sportists);
    while(sportist=(sportist_t*)getNext(league->sportists))
    {
      if(sportist->ID==sportistID){
	       return sportist;
      }
    }
}*/
/*
team_t* getTeamByID(league_t* league, int teamID)
{
    team_t* team;
    reset(league->teams);
    while(team=getNext(league->teams)){
      if(team->ID==teamID){
	       return team;
      } 
    }
}*/
/*
user_t* getUserByID(int userID)
{
    user_t* user;
    reset(users);
    while(user=getNext(users))
    {
      if(user->ID==userID)
      {
	       return user;
      }
    }
}

league_t* getLeagueByID(int leagueID)
{
    league_t* league;
    reset(leagues);
    while(league=getNext(leagues))
    {
      if(league->ID==leagueID)
      {
	       return league;
      }
    }
}*/