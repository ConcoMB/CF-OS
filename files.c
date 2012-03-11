#include "files.h"

void save_user(FILE* file,user_t user){
	FILE* users_file;
	users_file=fopen("./users.txt","a");
	fprintf(users_file, "%d %s %s\n", user.ID, user.name, user.password);
	fclose(users_file);	
}

void save_league(league_t league){
	FILE* league_file;
	league_file=fopen("./leagues.txt","a");
	fprintf(league_file, "%s\n", league.name);
	fclose(users_file);	
}
