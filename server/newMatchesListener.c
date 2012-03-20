#include "newMatchesListener.h"
/* falta hacer el metodo que actualiza los puntos de cada jugador cuando cambia la fecha de modificacion del archivo*/
void newMatchesListener(){
	/* las variables lastModification y modified tienen q ser globales*/
	struct stat lastModification;
	int modified=1;
	/**/
	char filename[] = "./matches.txt";
	char timeStr[ 100 ] = "";
	struct stat fileStat;
	char datebuf [9];
	char timebuf [9];
	if (!stat(filename, &fileStat))
	{
		if(!modified){
			lastModification = fileStat;
			modified=1;
			actualizeSportistsPoints();
		}
		else
		{
			if(difftime(lastModification.st_mtime,fileStat.st_mtime)!=0.0){
				actualizeSportistsPoints();
			}
		}
	}
	else
	{
		printf("error getting atime\n");
	}


}

void actualizeSportistsPoints(){

}
int main(void){
	newMatchesListener();
}
