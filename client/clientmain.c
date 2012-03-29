#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "../msg.h"
#include "../common.h"
#include "connection.h"
#define QUIT 12345

void sighandler(int sig);
void userLog(int msgID);
void start(int msgID);
void makeDefConnection(int * msgID);
void* channel;

int main()
{
	int msgID;
	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
	makeDefConnection(&msgID);
	connectClient(msgID,&channel);
	while(1){
		userLog(msgID);
	}
}


void makeDefConnection(int * msgID)
{
	int aux= NEWCLIENT;
	void* defChannel;
	defChannel=connectChannel(DEFAULTID+1);

	sndMsg(defChannel, (void*)&aux, sizeof(int));
	printf("mande\n");
	rcvMsg(defChannel, (void*)msgID, sizeof(int));
	printf("recibi msgid %d\n", *msgID);
	disconnect(defChannel);

}

void userLog(int msgID)
{
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
			sndMsg(channel, (void*)&aux, sizeof(int));
			printf("name:\n");
			scanf("%s", name);
			sndString(channel, name);
			printf("password:\n");
			scanf("%s", password);
			sndString(channel,password);
			rcvMsg(channel, (void*)&handshake, sizeof(int));
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
			sndMsg(channel, (void*)&aux, sizeof(int));
			printf("Enter new name:\n");
			scanf("%s", name);
			sndString(channel, name);
			printf("password:\n");
			scanf("%s", password);
			sndString(channel, password);
			rcvMsg(channel, (void*)&handshake, sizeof(int));
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
	char* args[6];
	char* path;
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
			path="./listleagues";
			args[0]="listleagues";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "listteams")==0)
		{
			path="./listteams";
			args[0]="listteams";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "listtrades")==0)
		{
			path="./listtrades";
			args[0]="listtrades";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "leagueshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID );
			sprintf(auxStr2, "%d", LEAGUE_SHOW);
			sprintf(auxStr3, "%d", END_LEAGUE_SHOW);
			path="./ltShow";
			args[0]="ltShow";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=auxStr2;
			args[4]=auxStr3;
			args[5]=NULL;
		}
		else if(strcmp(string, "teamshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			sprintf(auxStr2, "%d", TEAM_SHOW);
			sprintf(auxStr3, "%d", END_TEAM_SHOW);
			path="./ltShow";
			args[0]="ltShow";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=auxStr2;
			args[4]=auxStr3;
			args[5]=NULL;
		}
		else if(strcmp(string, "tradeshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./tradeShow";
			args[0]="tradeShow";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else if(strcmp(string, "draft")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./draft";
			args[0]="draft";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}

		else if(strcmp(string, "trade")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./makeTrade";
			args[0]="makeTrade";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else if(strcmp(string, "tradewithdraw")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			command=TRADE_WD;
			sprintf(auxStr2, "%d", command);
			path="./tradeAW";
			args[0]="tradeAW";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=auxStr2;
			args[4]=NULL;
		}
		else if(strcmp(string, "tradeaccept")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			command=TRADE_YES;
			sprintf(auxStr2, "%d", command);
			path="./tradeAW";
			args[0]="tradeAW";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=auxStr2;
			args[4]=NULL;
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
			path="./tradeNegociate";
			args[0]="tradeNegociate";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else if(strcmp(string, "createleague")==0)
		{
			path="./makeLeague";
			args[0]="makeLeague";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "joinleague")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./joinLeague";
			args[0]="joinLeague";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else
		{
			printf("invalid command\n");
		}
		if(command!=QUIT)
		{
			if(fork())
			{
				wait((int*) 0);
			}
			else
			{
				execv(path, args);
			}
		}
	}
	while(command!=QUIT);
	command = LOG_OUT;
	sndMsg(channel, (void*)&command, sizeof(int));
	return;
}

void sighandler(int sig)
{
    disconnect(channel);
    exit(0);
}
