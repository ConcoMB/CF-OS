#define ARRAY_SIZE 20

void resizeUser() {
	user_t ** auxUsers;
	auxUsers = realloc(users, sizeof(user_t*)*( uCant + ARRAY_SIZE)));
	if (auxUsers) {
		users = auxUsers;
	}
}

void resizeLeagues() {
	leagues_t ** auxLeagues;
	auxLeagues = realloc(leagues, sizeof(user_t*)*( lCant + ARRAY_SIZE)));
	if (auxLeagues) {
		leagues = auxLeagues;
	}
}

void resizeTeams(league_t * league) {
	teams_t ** auxTeams;
	auxTeams = realloc(league->teams, sizeof(user_t*)*( leagues->tCant + ARRAY_SIZE)));
	if (auxTeams) {
		league->teams = auxTeams;
	}
}

void resizeSportist(league_t * league) {
	sportist_t ** auxSportist;
	auxSportists = realloc(league->sportists, sizeof(user_t*)*( uCant + ARRAY_SIZE)));
	if (auxUsers) {
		league-> = auxSportists;
	}
}

void newUser(user_t* user) {
	if(users%ARRAY_SIZE==0){
		resizeUsers();
	}
	users[uCant++] = user;
}

void newLeague(league_t* league) {
	if(lCant%ARRAY_SIZE ==0){
		resizeLeagues();
	}
	leagues[lCant++] = league;
}

void newTeam(league_t * league, team_t * team) {
	if(league->tCant%ARRAY_SIZE==0){
		resizeTeams(league);
	}
	league->teams[league->tCant++] = team;
}

void newSportist(league_t * league, sportist_t * sportist) {
	if(league->sCant%ARRAY_SIZE==0){
		resizeSportists(league);
	}
	league->sportists[league->sCant++] = sportist;
}
