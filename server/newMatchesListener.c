#include "newMatchesListener.h"

//void printAll(void);

void * newMatchesListener() {
	while (1) {
		struct dirent * entry;
		DIR * dir = opendir("./matches");

		while ((entry = readdir(dir)) != NULL) {
			char matchFilePath[100];
			sprintf(matchFilePath, "./matches/%s", entry->d_name);
			FILE * matchFile = fopen(matchFilePath, "r");
			if (matchFile != NULL) {
				int sportistID = 0, sportistPoints = 0,j=0;
				while (fscanf(matchFile, "%d %d\n", &sportistID,&sportistPoints) != EOF) {
					if (sportistID >= 0 && sportistID < CANT_SPORTIST) {
						updateSportistPoints(sportistID, sportistPoints);
					}
				}
				fclose(matchFile);
				j=unlink(matchFilePath);
				printf("valor q retorna unlink %d, d_name: %s\n",j, entry->d_name);
			}
		}
		//printAll();
		closedir(dir);
		sleep(5);
	}

}

void updateSportistPoints(int id, int score) {
	int lid;
	for (lid = 0; lid < lCant; lid++) {
		if(leagues[lid]->sportists[id] != NULL){
			leagues[lid]->sportists[id]->score += score;
		}
	}
}
/*
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
      printf("	%d %d %s %d\n",sportist->ID, sportist->team->ID, sportist->name, sportist->score);
    }
    printf("	-- TRADES -- \n");
    trade_t* trade;
    reset(leagues[i]->trades);
    while(trade=(trade_t*)getNext(leagues[i]->trades))
    {
      printf("	%d %d %d %d\n",trade->ID, trade->from->ID, trade->to->ID, trade->offer->ID, trade->change->ID);
    }
  }
}*/
