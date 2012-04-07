#include "newMatchesListener.h"

void * newMatchesListener() 
{
	while (1) 
	{
		struct dirent * entry;
		DIR * dir = opendir("./matches");
		if (dir == NULL) 
		{
			printf("Falta el directorio matches\n");
			exit(0);
		} 
		else
		{

			while ((entry = readdir(dir)) != NULL) 
			{
				char matchFilePath[100];
				sprintf(matchFilePath, "./matches/%s", entry->d_name);
				FILE * matchFile = fopen(matchFilePath, "r");
				if (matchFile != NULL)
				{
					int sportistID = 0, sportistPoints = 0;
					while (fscanf(matchFile, "%d %d\n", &sportistID, &sportistPoints) != EOF) 
					{
						if (sportistID >= 0 && sportistID < CANT_SPORTIST) 
						{
							updateSportistPoints(sportistID, sportistPoints);
						}
					}
					fclose(matchFile);
					unlink(matchFilePath);
					//printf("valor q retorna unlink %d, d_name: %s\n",j, entry->d_name);
				}
			}
			//printAll();
			closedir(dir);
		}
	sleep(5);
	}

}

void updateSportistPoints(int id, int val) 
{
	int lid;
	sportist_t* sport;
	for (lid = 0; lid < lCant; lid++) 
	{
		sport=leagues[lid]->sportists[id];
		sport->score+=val;
		if(sport->team!=NULL)
		{
			sport->team->points+=val;
		}
	}
}
