#include "files.h"

/* SAVE */

void saveUser(FILE* file,user_t* user){
	FILE* usersFile;
	usersFile=fopen("./users.txt","a");
	fprintf(usersFile, "%d %s %s\n", user->ID, user->name, user->password);
	fclose(usersFile);	
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

void load_user(user_t* user){
	FILE* usersFile;
	usersFile=fopen("./users.txt","r");
	fscanf(usersFile, "%d %s %s\n", &user->ID, user->name, user->password);
	fclose(usersFile);	
}

void load_league(league_t* league){
	FILE* leagueFile;
	leagueFile=fopen("./leagues.txt","r");
	fscanf(leagueFile, "%d %s\n", &league->ID, league->name);
	fclose(leagueFile);
}

void load_team(team_t* team){
	FILE* teamFile;
	int userID, leagueID, sportistID;
	user_t* user;
	league_t* league;
	
	/* Read data from file */
	teamFile=fopen("./teams.txt","r");
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
	while(sportistID=fscanf(
	
	fclose(teamFile);
}

