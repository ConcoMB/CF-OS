#include "newMatchesListener.h"

league_t** leagues;
int lCant, uCant;
user_t** users;
listADT clients;
int nextUserID = 0;
int nextLeagueID = 0;
int nextClientID = 1;
void printAll(void);

void newMatchesListener() {
	while (1) {
		struct dirent * entry;
		DIR * dir = opendir("./matches");

		while ((entry = readdir(dir)) != NULL) {
			FILE * matchFile = fopen(entry->d_name, "r");
			if (matchFile != NULL) {

				int sportistID = 0, sportistPoints = 0,j=0;
				while (fscanf(matchFile, "%d %d\n", &sportistID,&sportistPoints) != EOF) {
					if (sportistID >= 0 && sportistID < CANT_SPORTIST) {
						updateSportistPoints(sportistID, sportistPoints);
					}
				}
				j=unlink(entry->d_name);
				printf("%d\n",j);
			}
		}
		printAll();
		closedir(dir);
		sleep(5);
	}

}

void updateSportistPoints(int id, int score) {
	int lid;
	for (lid = 0; lid < lCant; lid++) {
		leagues[lid]->sportists[id]->score += score;
	}
}

void printAll()
{
  int i;
  printf("-- USERS --\n");
  for(i=0;i<uCant;i++)
  {
    printf("%d %s %s\n",users[i]->ID, users[i]->name, users[i]->password);
  }
  printf("-- LEAGUES --\n");
  for(i=0;i<lCant;i++)
  {
    int j;
    printf("%d %s %s\n",leagues[i]->ID, leagues[i]->name, leagues[i]->password);
    printf("	-- TEAMS --\n");
    for(j=0;j<leagues[i]->tCant;j++){
      team_t* team=leagues[i]->teams[j];
      printf("	%d %d %s\n",team->ID, team->user->ID, team->name);
    }
    printf("	-- SPORTIST -- \n");
    for(j=0;j<CANT_SPORTIST&&leagues[i]->sportists[j];j++)
    {
      sportist_t* sportist=leagues[i]->sportists[j];
      printf("	%d %d %s\n",sportist->ID, sportist->team->ID, sportist->name);
    }
    printf("	-- TRADES -- \n");
    trade_t* trade;
    reset(leagues[i]->trades);
    while(trade=(trade_t*)getNext(leagues[i]->trades))
    {
      printf("	%d %d %d %d\n",trade->ID, trade->from->ID, trade->to->ID, trade->offer->ID, trade->change->ID);
    }
  }
}

int main(void) {
	loadAll();
	newMatchesListener();
}
