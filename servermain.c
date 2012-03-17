#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include "league.h"

int main()
{
	listADT users, leagues, teams, trades, connectedUsers;
	/*loadUsers(users);
	loadLeagues(leagues);
	loadTrades(trades);
	loadTeam(teams);
*/
	//thread q controle los q se conectan
	int fdsend, fdrcv;
	char buffer[5];
	user_t * user1= malloc(sizeof(user_t));
	strcpy(user1->name, "caco");
	strcpy(user1->password, "cuca");
	//insert(users, user1);
	mkfifo("./fifos",0666);
	fdrcv=open("./fifoc", O_RDONLY);
	fdsend=open("./fifos", O_WRONLY);
	read(fdrcv, buffer, 5);
	if(strcmp(user1->name, buffer)==0){
		printf("usuario encontrado\n");
		read(fdrcv, buffer, 5);
		if(strcmp(user1->password, buffer)==0){
			printf("login correcto\n");
			strcpy(buffer, "si");
			write(fdsend, buffer, 5);
		}

	}
	close(fdsend);
	close(fdrcv);

	return 0;
}