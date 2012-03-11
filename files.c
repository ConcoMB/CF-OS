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
	fprintf(tradeFile, "%d %d %d %d\n", trade->from->ID, trade->to->ID, trade->offer->ID, trade->change->ID);
	fclose(tradeFile);
}

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
	user_t* user;
	league_t* league;
	team=malloc(sizeof(team_t));
	
	/* Read data from file */
	if(fscanf(teamFile, "%d %s %d %d %d", &team->ID, team->name, &userID, &leagueID, &team->points)){
		/* Link user */
		user=getUserByID(userID);
		team->user=user;
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
	teamFile=fopen("./teams.txt","r");
	
	fclose(teamFile);
}

static trade_t* loadTrade(team_t* team){
	
	int userID, leagueID, sportistID;
	user_t* user;
	league_t* league;
	trade=malloc(sizeof(team_t));
	
	/* Read data from file */
	
	fscanf(teamFile, "%d %s %d %d %d", &team->ID, team->name, &userID, &leagueID, &team->points);
	
	/* Link user */
	user=getUserByID(userID);
	team->user=user;
	insertElem(user->teams,team);
	
	/* Link league */
	league=getLeagueByID(leagueID);
	team->league=league;
	insertElem(league->teams, team);
	
	/* Read sportists from file */
	team->sportists=newList(sizeof(sportist_t),NULL);
	while(fscanf(teamFile,"%d",&sportistID)){
		insertElem(team->sportists, getSportistByID(sportistID));
	}
}
