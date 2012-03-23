#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "join.h"
#include "display.h"
#include "league.h"
#include <sys/shm.h>

int readFD, writeFD;
client_t* myClient;

void makeConnection();
void start();
void logClient();

void* clientAtt(void* arg)
{
	printf("entre al attendant\n");
	myClient=(client_t*)arg;
	makeConnection();
	logClient();
	start();
}

void logClient()
{
	int aux, loged=0, msg;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	while(!loged)
	{
		rcvMsg(readFD,(void*)&msg, sizeof(int));
		printf("%d\n", msg);
		rcvString(readFD, name);
		printf("%s\n", name);
		rcvString(readFD, password);
		printf("%s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password, myClient))==0)
			{
				loged=1;
			}
			sndMsg(writeFD, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			printf("Hago el signup\n");
			aux=signUp(name, password);
			sndMsg(writeFD, (void*)&aux, sizeof(int));
			if(aux==0)
			{
			    loged=1;
			    printf("voy al login\n");
			    logIn(name, password, myClient);
			}
		}
	}
}

void start()
{
	int msg;
	while(1)
	{
		rcvMsg(readFD,(void*)&msg, sizeof(int));
		switch(msg)
		{
			case SEND_LEAGUE:
				listLeagues(writeFD);
				break;
			case SEND_TEAM:
				listTeam(myClient->user ,writeFD);
				break;
			case SEND_TRADE:
				listTrades(myClient->user, writeFD);
				break; 
			case LEAGUE_SHOW:
				rcvMsg(readFD, (void*)&msg, sizeof(int));
				if(msg<lCant&&msg>=0)
				{
					leagueShow(leagues[msg], writeFD);
				}
				else
				{
					msg=LEAGUE_ID_INVALID;
					sndMsg(writeFD, (void*)&msg, sizeof(int));
				}
				break;
		}
	}
}

void makeConnection()
{
	char readChannel[4], writeChannel[4];
	int id=myClient->ID;
	sprintf(writeChannel, "%c%d", 's', id);
	create(writeChannel);
	writeFD=connect(writeChannel, O_WRONLY);
	sprintf(readChannel, "%c%d", 'c', id);
	create(readChannel);
	readFD=connect(readChannel, O_RDONLY);
}