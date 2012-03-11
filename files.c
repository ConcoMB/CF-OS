#include "files.h"

void save_user(FILE* file,user_t* user){
	FILE* users_file;
	users_file=fopen("./users.txt","a");
	fprintf(users_file, "%d %s %s\n", user->ID, user->name, user->password);
	fclose(users_file);	
}

void save_league(league_t* league){
	FILE* league_file;
	league_file=fopen("./leagues.txt","a");
	fprintf(league_file, "%d %s\n", league->ID, league->name);
	fclose(league_file);
}

void save_team(team_t* team){
	FILE* team_file;
	team_file=fopen("./teams.txt","a");
	fprintf(team_file, "%d %s %d %d %d", team->ID, team->name, team->user->ID, team->league->ID ,team->points);
	sportist_t* sportist;
	reset(team->sportists);
	while((sportist=(sportist_t*)getNext(team->sportists))!=NULL){
		fprintf(team_file," %s",sportist);
	}
	fprintf(team_file,"\n");
	fclose(team_file);
}

void save_trade(trade_t* trade){
	FILE* trade_file;
	trade_file=fopen("./leagues.txt","a");
	fprintf(trade_file, "%d %s\n", league->ID, league->name);
	fclose(trade_file);
}
