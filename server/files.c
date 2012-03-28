#include "files.h"

/* SAVE */

void saveAll(){
    saveUsers();
    saveLeagues();
}

void saveUsers(){
    FILE* userFile;
    user_t* user;
    userFile=fopen("./data/users.txt","w");
    int uid;
    for(uid=0; uid<uCant;uid++)
    {
		user=users[uid];
		saveUser(userFile, user);
    }
    fclose(userFile);
}

static void saveUser(FILE* userFile, user_t* user){	
	fprintf(userFile, "%d %s %s\n", user->ID, user->name, user->password);
}

void saveLeagues(){
    FILE* leagueFile, *teamFile, *sportistFile, *tradeFile;
    league_t* league;
    team_t* team;
    trade_t* trade;
    sportist_t* sportist;
    leagueFile=fopen("./data/leagues.txt","w");
    int lid;
    for(lid=0;lid<lCant;lid++)
    {
		league=leagues[lid];
		saveLeague(leagueFile,league);
		char filename[30];
		sprintf(filename,"./data/%d_sportists.txt",league->ID);
		sportistFile=fopen(filename,"w");
		sprintf(filename,"./data/%d_teams.txt",league->ID);
		teamFile=fopen(filename,"w");
		sprintf(filename,"./data/%d_trades.txt",league->ID);
		tradeFile=fopen(filename,"w");
		reset(league->trades);
		while(trade=(trade_t*)getNext(league->trades)){
			saveTrade(tradeFile, trade);
		}
		int tid;
		for(tid=0;tid<league->tCant;tid++)
		{
			team=league->teams[tid];
			saveTeam(teamFile, team);
		}
		int sid;
		for(sid=0;sid<CANT_SPORTIST&&league->sportists[sid];sid++)
		{
			sportist=league->sportists[sid];
			saveSportist(sportistFile, sportist);
		}
		fclose(teamFile);
		fclose(sportistFile);
		fclose(tradeFile);
    }
    fclose(leagueFile);
}

static void saveLeague(FILE* leagueFile, league_t* league){
	int draft=0;
	if(league->draft)
	{
		draft=1;
	}
	fprintf(leagueFile, "%d %d %s %d %d %d %s\n", league->ID , league->nextTeamID, league->name, league->tMax, league->nextTradeID, draft, league->password);
}

void saveTeam(FILE* teamFile, team_t* team){
	fprintf(teamFile, "%d %s %d %d\n", team->ID, team->name, team->user->ID ,team->points);
}

void saveTrade(FILE* tradeFile, trade_t* trade){
	fprintf(tradeFile, "%d %d %d %d %d\n", trade->ID, trade->from->ID, trade->to->ID, trade->offer->ID, trade->change->ID);
}

void saveSportist(FILE* sportistFile, sportist_t* sportist){
	int teamID;
	if(sportist->team==NULL)
	{
		teamID=-1;
	}
	else
	{
		teamID=sportist->team->ID;
	}
	fprintf(sportistFile, "%d %d %d %s\n", sportist->ID, sportist->score, teamID, sportist->name);
}

/* LOAD */

void loadAll(){

	loadUsers();
	loadLeagues();
}

void loadUsers(){
	FILE* userFile;
	user_t* user;
	userFile=fopen("./data/users.txt","r");
	while(user=loadUser(userFile)){
		 newUser(user);
	}
	fclose(userFile);	
}

static user_t* loadUser(FILE* userFile){
	user_t* user;
	user=malloc(sizeof(user_t));
	if(fscanf(userFile, "%d %s %s\n", &user->ID, user->name, user->password)!=EOF){
		user->teams=newList(cmpTeam);
		return user;
	}
	free(user);
	return NULL;
}

void loadLeagues(){
	FILE* leagueFile;
	league_t* league;
	leagueFile=fopen("./data/leagues.txt","r");
	while(league=loadLeague(leagueFile)){
		newLeague(league);
	}
	fclose(leagueFile);
}

static league_t* loadLeague(FILE* leagueFile){
	league_t* league;
	int draft;
	league=malloc(sizeof(league_t));
	if(fscanf(leagueFile, "%d %d %s %d %d %d %s\n", &league->ID , &league->nextTeamID, league->name, &league->tMax, &league->nextTradeID, &draft, league->password)!=EOF){
		if(draft)
		{
			league->draft=malloc(sizeof(draft_t));
			league->draft->league=league;
		}
		if(league->password[0]=='0')
		{
			league->password[0]='\0';
		}
		loadTeams(league);
		loadSportists(league);
		league->trades=newList(cmpTrade);
		loadTrades(league);
		return league;
	}
	free(league);
	return NULL;
}

void loadTeams(league_t* league){
	FILE* teamFile;
	team_t* team;
	char filename[30];
	sprintf(filename,"./data/%d_teams.txt", league->ID);
	teamFile=fopen(filename, "r");
	while(team=loadTeam(teamFile, league)){
		newTeam(league, team);
	}
	fclose(teamFile);
}

static team_t* loadTeam(FILE* teamFile, league_t* league){

	int userID, leagueID, sportistID;
	team_t* team;
	team=malloc(sizeof(team_t));
	
	/* Read data from file */
	if(fscanf(teamFile, "%d %s %d %d\n", &team->ID, team->name, &userID, &team->points)!=EOF){
		/* Link user */
		team->user=users[userID];
		insert(team->user->teams,(void*)team);
		
		/* Link league */
		team->league=league;
		return team;
	}
	free(team);
	return NULL;	
}

void loadTrades(league_t* league){
	FILE* tradeFile;
	trade_t* trade;
	char filename[30];
	sprintf(filename,"./data/%d_trades.txt", league->ID);
	tradeFile=fopen(filename, "r");
	while(trade=loadTrade(tradeFile, league)){
		insert(league->trades, (void*)trade);
	}
	fclose(tradeFile);
}

static trade_t* loadTrade(FILE* tradeFile, league_t* league){
	
	int fromID, toID, offerID, changeID, leagueID;
	trade_t* trade;
	trade=malloc(sizeof(trade_t));
	
	/* Read data from file */
	
	if(fscanf(tradeFile, "%d %d %d %d %d\n", &trade->ID, &fromID, &toID, &offerID, &changeID)!=EOF){
		/* Link Users */
		trade->from=league->teams[fromID];
		trade->to=league->teams[toID];
		
		/* Link League */
		trade->league=league;
		
		/* Link Sportists */
		trade->offer=league->sportists[offerID];
		trade->change=league->sportists[changeID];
		return trade;
	}
	free(trade);
	return NULL;
}

void loadSportists(league_t* league){
	FILE* sportistFile; 
	sportist_t* sportist;
	char filename[30];
	sprintf(filename,"./data/%d_sportists.txt",league->ID);
	sportistFile=fopen(filename,"r");
	int sCant=0;
	while(sportist=loadSportist(sportistFile, league)){
		league->sportists[sCant++]=sportist;
	}
	fclose(sportistFile);
}

sportist_t* loadSportist(FILE* sportistFile, league_t* league){
	int sportistID, score, teamID;
	sportist_t* sportist;
	sportist=malloc(sizeof(sportist_t));
	if(fscanf(sportistFile,"%d %d %d %s\n", &sportist->ID, &sportist->score, &teamID, sportist->name)!=EOF){
		/* Link Team */
		if(teamID>=0)
		{
			sportist->team=league->teams[teamID];	
		}
		else
		{
			sportist->team=NULL;
		}
		return sportist;
	}
	free(sportist);
	return NULL;
}

void loadNewSportists(league_t* league){
	FILE* sportistFile;
	sportist_t* sportist;
	int i;
	sportistFile=fopen("./data/sportists.txt","r");
	for(i=0; i<CANT_SPORTIST;i++)
	{
		char name[SPORT_NAME_L];
		if(fscanf(sportistFile,"%s\n",name)!=EOF)
		{
			sportist=malloc(sizeof(sportist_t));
			strcpy(sportist->name,name);
			sportist->ID=i++;
			sportist->team=NULL;
		}
	}
	fclose(sportistFile);
}
