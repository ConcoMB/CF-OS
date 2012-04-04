#include <string.h>
#include <stdio.h>
#include "../msg.h"
#include "../common.h"
#include "shell.h"

void shell(int msgID)
{
	int command, auxID;
	char string[20], auxString[10], auxStr2[10], auxStr3[10], idStr[5];
	sprintf(idStr,"%d",msgID);
	char* args[6];
	char* path;
	int invalid;
	do
	{
		invalid=0;
		printf("type your command \n");
		scanf("%s", string);
		if(strcmp(string, "quit")==0)
		{
			command=QUIT;
		}
		else if(strcmp(string, "listleagues")==0)
		{
			path="./listleagues.e";
			args[0]="listleagues.e";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "listteams")==0)
		{
			path="./listteams.e";
			args[0]="listteams.e";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "listtrades")==0)
		{
			path="./listtrades.e";
			args[0]="listtrades.e";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "leagueshow")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID );
			sprintf(auxStr2, "%d", LEAGUE_SHOW);
			sprintf(auxStr3, "%d", END_LEAGUE_SHOW);
			path="./ltShow.e";
			args[0]="ltShow.e";
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
			path="./ltShow.e";
			args[0]="ltShow.e";
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
			path="./tradeShow.e";
			args[0]="tradeShow.e";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else if(strcmp(string, "draft")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./draft.e";
			args[0]="draft.e";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}

		else if(strcmp(string, "trade")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./makeTrade.e";
			args[0]="makeTrade.e";
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
			path="./tradeAW.e";
			args[0]="tradeAW.e";
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
			path="./tradeAW.e";
			args[0]="tradeAW.e";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=auxStr2;
			args[4]=NULL;
		}
		else if(strcmp(string, "tradenegociate")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./tradeNegociate.e";
			args[0]="tradeNegociate.e";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else if(strcmp(string, "createleague")==0)
		{
			path="./makeLeague.e";
			args[0]="makeLeague.e";
			args[1]=idStr;
			args[2]=NULL;
		}
		else if(strcmp(string, "joinleague")==0)
		{
			scanf("%d", &auxID);
			sprintf(auxString, "%d",auxID);
			path="./joinLeague.e";
			args[0]="joinLeague.e";
			args[1]=idStr;
			args[2]=auxString;
			args[3]=NULL;
		}
		else
		{
			invalid=1;
			printf("invalid command\n");
		}
		if(!invalid && command!=QUIT)
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
