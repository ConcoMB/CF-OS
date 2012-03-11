#include "files.h"

/* SAVE */

void saveUser(FILE* file,user_t* user){
	FILE* userFile;
	userFile=fopen("./users.txt","a");
	fprintf(userFile, "%d %s %s\n", user->ID, user->name, user->password);
	fclose(userFile);	
}

void saveLeague(league_t* league){
	FILE* leagueFile;
	leagueFile=fopen("./leagues.txt","a");
	fprintf(leagueFile, "%d %s\n", league->ID, league->name);
	fclose(leagueFile);
}

void saveTeam(team_t* team){
	FILE* teamFile;
	teamFile=fopen("./teams.txt","a");
	fprintf(teamFile, "%d %s %d %d %d", team->ID, team->name, team->user->ID, team->league->ID ,team->points);
	sportist_t* sportist;
	reset(team->sportists);
	while((sportist=(sportist_t*)getNext(team->sportists))!=NULL){
		fprintf(teamFile," %d",sportist->ID);
	}
	fprintf(teamFile,"\n");
	fclose(teamFile);
}

void saveTrade(trade_t* trade){
	FILE* tradeFile;
	tradeFile=fopen("./trades.txt","a");
	fprintf(tradeFile, "%d %d %d %d %d %d\n", trade->ID, trade->league->ID, trade->from->ID, trade->to->ID, trade->offer->ID, trade->change->ID);
	fclose(tradeFile);
}

/*void saveScores(league_t league){
	FILE* scoreFile; 
	char filename[20];
	int sportistID, score, teamID;
	sprintf(filename,"%d_scores.txt",league->ID);
	scoreFile=fopen(filename,"w");
	sportist_t* sportist;
	while(sportist=(sportist_t*)getNext(league->sportists)){
	}
	fclose(scoreFile);
}*/

/* LOAD */

void loadUsers(ListADT users){
	FILE* userFile;
	user_t* user;
	users=newList(sizeof(user*),NULL);
	userFile=fopen("./users.txt","r");
	while(user=loadUser(userFile)){
		insertElem(users,user);
	}
	fclose(userFile);	
}

static user_t* loadUser(FILE* userFile){
	user_t* user;
	user=malloc(sizeof(user_t));
	if(fscanf(userFile, "%d %s %s\n", &user->ID, user->name, user->password)){
		return user;
	}
	free(user);
	return NULL;
}

void loadLeagues(ListADT leagues){
	FILE* leagueFile;
	league_t league;
	leagueFile=fopen("./leagues.txt","r");
	leagues=newList(sizeof(leagues*),NULL);
	while(league=loadLeague(leagueFile)){
		insertElem(leagues,league);
	}
	fclose(leagueFile);
}

static league_t* loadLeague(FILE* leagueFile){
	league_t* league;
	league=malloc(sizeof(league_t));
	if(fscanf(leagueFile, "%d %s\n", &league->ID, league->name)){
		league->sportists=newList(sizeof(sportist_t*), NULL);
		league->teams=newList(sizeof(team_t*), NULL);
		league->trades=newList(sizeof(trade_t*), NULL);
		return league;
	}
	free(league);
	return NULL;
}

void loadSportists(ListADT sportists){
	FILE* sportistFile;
	sportist_t* sportist;
	int sportistID;
	
	sportistFile=fopen("sportists.txt","r");
	while(scanf(sportistFile,"%d %s",&sportistID)){
		sportist=malloc(sizeof(sportist_t));
		sportist->ID=sportistID;
		fgets(sportist->name, 30, sportistFile);
	}
	
	fclose(sportistFile);
}

void loadTeams(ListADT teams){
	FILE* teamFile;
	team_t* team;
	teamFile=fopen("./teams.txt","r");
	while(team=loadTeam(teamFile));
	fclose(teamFile);
}

static team_t* loadTeam(FILE* teamFile){

	int userID, leagueID, sportistID;
	league_t* league;
	user_t* user;
	team=malloc(sizeof(team_t));
	
	/* Read data from file */
	if(fscanf(teamFile, "%d %s %d %d %d", &team->ID, team->name, &userID, &leagueID, &team->points)){
		/* Link user */
		user=getUserByID(userID);
		trade->user=user;
		insertElem(user->teams,team);
		
		/* Link league */
		league=getLeagueByID(leagueID);
		team->league=league;
		insertElem(league->teams, team);
		
		/* Read sportists from file */
		team->sportists=newList(sizeof(sportist_t*),NULL);
		while(fscanf(teamFile,"%d",&sportistID)){
			insertElem(team->sportists, getSportistByID(sportistID));
		}
		return team;
	}
	free(team);
	return NULL;	
}

void loadTrades(){
	FILE* tradeFile;
	trade_t* trade;
	teamFile=fopen("./teams.txt","r");
	while(trade=loadTrade(tradeFile));
	fclose(teamFile);
}

static trade_t* loadTrade(FILE* tradeFile){
	
	int fromID, toID, offerID, changeID, leagueID;
	league_t* league;
	trade=malloc(sizeof(trade_t));
	
	/* Read data from file */
	
	if(fscanf(tradeFile, "%d %d %d %d %d %d", trade->ID, &leagueID, &fromID, &toID, &offerID, &changeID)){
		/* Link Users */
		trade->from=getUserByID(fromID);
		trade->to=getUserByID(toID);
		
		/* Link Sportists */
		trade->offer=loadSportistByID(leagueID,offerID);
		trade->change=loadSportistByID(leagueID,changeID);
		
		/* Link League */
		insertElem(getLeagueByID(leagueID)->trades,trade);
		return trade;
	}
	free(trade);
	return NULL;
}

void loadScores(league_t league){
	FILE* scoreFile; 
	char filename[20];
	int sportistID, score, teamID;
	sprintf(filename,"%d_scores.txt",league->ID);
	scoreFile=fopen(filename,"r");
	while(fscanf(scoreFile,"%d %d %d", &sportistID, &score, &teamID)){
		sportist_t* sportist;
		sportist=getSportistByID(league->ID, sportistID);
		sportist->score=score;
		sportist->team=getTeamByID(league->ID, teamID);
	}
	fclose(scoreFile);
}
