#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../msg.h"
#include "../common.h"
#include "connection.h"
#define QUIT 12345


void userLog(int msgID);
void start(int msgID);
void makeDefConnection(int * msgID);
void* readFD, *writeFD;

int main()
{
	int msgID;
	makeDefConnection(&msgID);
	while(1){
		userLog(msgID);
	}
}

void makeDefConnection(int * msgID)
{
	int aux= NEWCLIENT;
	char defWChannel[3], defRChannel[3];
	void* defRead, *defWrite;
	sprintf(defWChannel, "%c%d", 'c', DEFAULTID);
	sprintf(defRChannel, "%c%d", 's', DEFAULTID);
	defWrite=connectChannel(defWChannel, O_WRONLY);
	defRead=connectChannel(defRChannel, O_RDONLY);
	
	sndMsg(defWrite, (void*)&aux, sizeof(int));
	printf("mande\n");
	rcvMsg(defRead, (void*)msgID, sizeof(int));
	printf("recibi msgid %d\n", *msgID);
}

void userLog(int msgID)
{
	connectClient(msgID,&writeFD,&readFD);
	int loged=0;
	while(!loged)
	{
		int handshake;
		char command[10], name[NAME_LENGTH], password[NAME_LENGTH];
		printf("Type login or signup\n");
		scanf("%s", command);
		if(strcmp(command, "login")==0)
		{
			int aux=LOGIN;
			sndMsg(writeFD, (void*)&aux, sizeof(int));
			printf("name:\n");
			scanf("%s", name);
			sndString(writeFD, name);
			printf("password:\n");
			scanf("%s", password);
			sndString(writeFD,password);
			rcvMsg(readFD, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case INCORRECT_PASSWORD:
					printf("incorrect password\n");
					break;
				case USER_NOT_FOUND:
					printf("user unknown\n");
					break;
				default:
					loged=1;
			}
		}
		else if(strcmp(command, "signup")==0)
		{
			int aux=SIGNUP;
			sndMsg(writeFD, (void*)&aux, sizeof(int));
			printf("Enter new name:\n");
			scanf("%s", name);
			sndString(writeFD, name);			
			printf("password:\n");
			scanf("%s", password);
			sndString(writeFD, password);
			printf("recibiendo handshake\n");
			rcvMsg(readFD, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case NAME_OCCUPIED:
					printf("User name already taken, choose an other\n");
					break;
				case NAME_OR_PASSWORD_TOO_LARGE:
					printf("The length of the user name and the password must be lower than 15 characters\n");
					break;
				default:
					printf("te incribiste piibeee\n");
					loged=1;
			}
		}
		else
		{
			printf("invalid command\n");
		}
	}
	start(msgID);
}

void start(int msgID)
{
	int command, auxID, auxOffer, auxChange;
	char string[20], auxString[10], auxStr2[10], auxStr3[10], auxStr4[10], idStr[5]; 
	sprintf(idStr,"%d",msgID);
	do
	{
		printf("type your command \n");
		scanf("%s", string);
		if(strcmp(string, "quit")==0)
		{
			command=QUIT;
		}
		else if(strcmp(string, "listleagues")==0)
		{
			if(fork())
			{
				wait((int*) 0);
			}
			else
			{
				execl("./listleagues", "listleagues",idStr, NULL);
			}
		}
		else if(strcmp(string, "listteams")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./listteams", "listteams", idStr, NULL);
			}
		}
		else if(strcmp(string, "listtrades")==0)
		{
			printf("antes de preguntar por el pid de listtrades");
			sleep(5);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				printf("antes del excl de listtrades");
				execl("./listtrades", "listtrades", idStr, NULL);
			}
		}
		else if(strcmp(string, "leagueshow")==0)
		{	
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID );
			sprintf(auxStr2, "%d", LEAGUE_SHOW);
			sprintf(auxStr3, "%d", END_LEAGUE_SHOW);

			printf("%d es el id de la liga\n", auxID);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./ltShow", "ltShow", idStr, auxString, auxStr2, auxStr3,  NULL);
			}
		}
		else if(strcmp(string, "teamshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			sprintf(auxStr2, "%d", TEAM_SHOW);
			sprintf(auxStr3, "%d", END_TEAM_SHOW);

			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./ltShow", "ltShow", idStr, auxString, auxStr2, auxStr3, NULL);
			}
		}
		else if(strcmp(string, "tradeshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./tradeShow", "tradeShow", idStr, auxString, NULL);
			}
		}
		else if(strcmp(string, "draft")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./draft", "draft", idStr, auxString, NULL);
			}
		}
		
		else if(strcmp(string, "trade")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./makeTrade", "makeTrade", idStr, auxString, NULL);
			}
		}
		else if(strcmp(string, "tradewithdraw")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			command=TRADE_WD;
			sprintf(auxStr2, "%d", command);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./tradeAW", "tradeAW", idStr, auxString, auxStr2, NULL);
			}
		}
		else if(strcmp(string, "tradeaccept")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			command=TRADE_YES;
			sprintf(auxStr2, "%d", command);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./tradeAW", "tradeAW", idStr, auxString, auxStr2, NULL);
			}
		}
		else if(strcmp(string, "tradenegociate")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("tradeNegociate", "tradeNegociate", idStr, auxString, NULL);
			}
		}
		else if(strcmp(string, "createleague"))
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("makeLeague.c", "makeLeague", idStr , NULL);
			}

		}
		else if(strcmp(string, "joinleague"))
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("joinLeague.c", "joinLeague", idStr, auxString, NULL);
			}

		}
		else 
		{
			printf("invalid command, type 'help' for help\n");
		}
	}
	while(command!=QUIT);
}
