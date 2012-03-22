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

int read, write;
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
		rcvMsg(read,(void*)&msg, sizeof(int));
		printf("%d\n", msg);
		rcvString(read, name);
		printf("%s\n", name);
		rcvString(read, password);
		printf("%s\n", password);
		if(msg==LOGIN)
		{
			if((aux=logIn(name,password, myClient))==0)
			{
				loged=1;
			}
			sndMsg(write, (void*)&aux, sizeof(int));
		}
		else if(msg==SIGNUP)
		{
			printf("Hago el signup\n");
			aux=signUp(name, password);
			sndMsg(write, (void*)&aux, sizeof(int));
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
		rcvMsg(read,(void*)&msg, sizeof(int));
		switch(msg)
		{
			case SEND_LEAGUE:
				listLeagues(write);
				break;
			case SEND_TEAM:
				listTeam(myClient->user ,write);
				break;
			case SEND_TRADE:
				listTrades(myClient->user, write);
				break; 
			case LEAGUE_SHOW:
				rcvMsg(read, (void*)&msg, sizeof(int));
				if(msg<lCant&&msg>=0)
				{
					leagueShow(leagues[msg], write);
				}
				else
				{
					msg=LEAGUE_ID_INVALID;
					sndMsg(write, (void*)&msg, sizeof(int));
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
	write=connect(writeChannel, O_WRONLY);
	sprintf(readChannel, "%c%d", 'c', id);
	create(readChannel);
	read=connect(readChannel, O_RDONLY);
}