#include "arrays.h"

void resizeUsers() 
{
	user_t ** auxUsers;
	auxUsers = realloc(users, sizeof(user_t*)*( uCant + ARRAY_SIZE));
	if (auxUsers) 
	{
		users = auxUsers;
	}
}

void resizeLeagues() 
{
	league_t ** auxLeagues;
	auxLeagues = realloc(leagues, sizeof(user_t*)*( lCant + ARRAY_SIZE));
	if (auxLeagues) 
	{
		leagues = auxLeagues;
	}
}

void resizeTeams(league_t * league)
 {
	team_t ** auxTeams;
	auxTeams = realloc(league->teams, sizeof(user_t*)*(league->tCant + ARRAY_SIZE));
	if (auxTeams) 
	{
		league->teams = auxTeams;
	}
}

void newUser(user_t* user) 
{
	if(uCant%ARRAY_SIZE==0)
	{
		resizeUsers();
	}
	users[user->ID] = user;
	uCant++;
}

void newLeague(league_t* league) {

	if(lCant%ARRAY_SIZE ==0)
	{
		resizeLeagues();
	}
	leagues[league->ID] = league;
	lCant++;
}

void newTeam(league_t * league, team_t * team) 
{
	if(league->tCant%ARRAY_SIZE==0)
	{
		resizeTeams(league);
	}
	league->teams[team->ID] = team;
	league->tCant++;
}
