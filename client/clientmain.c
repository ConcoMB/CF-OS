#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../msg.h"
#include "../common.h"
#define QUIT 12345


void userLog(int msgID);
void start();
void makeDefConnection(int * msgID);
int readFD, writeFD;

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
	int defRead, defWrite;
	sprintf(defWChannel, "%c%d", 'c', DEFAULTID);
	sprintf(defRChannel, "%c%d", 's', DEFAULTID);
	defWrite=connect(defWChannel, O_WRONLY);
	defRead=connect(defRChannel, O_RDONLY);
	
	sndMsg(defWrite, (void*)&aux, sizeof(int));
	printf("mande\n");
	rcvMsg(defRead, (void*)msgID, sizeof(int));
	printf("recibi msgid %d\n", *msgID);
}

void userLog(int msgID)
{
	char readChannel[4], writeChannel[4];
	sprintf(readChannel, "%c%d", 's', msgID);
	sprintf(writeChannel, "%c%d", 'c', msgID);
	create(readChannel);
	create(writeChannel);
	readFD=connect(readChannel, O_RDONLY);
	writeFD=connect(writeChannel, O_WRONLY);
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
	start();
}

void start()
{
	int command;
	printf("%d %d\n", readFD, writeFD);
	char string[20], stringR[10], stringW[10]; 
	sprintf(stringW, "%d", writeFD);
	sprintf(stringR, "%d", readFD);
	do
	{
		printf("type your command \n");
		scanf("%s", string);
		if(strcmp(string, "quit")==0)
		{
			command==QUIT;
		}
		else if(strcmp(string, "listleagues")==0)
		{
			int cid=fork();
			if(cid)
			{
				waitpid(cid, NULL, 0);
			}
			else
			{
				printf("me forkeo\n");
				execl("./listleagues", "listleagues",stringW, stringR, NULL);
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
				execl("./listteams", "listteams", writeFD, readFD, NULL);
			}
		}
		else if(strcmp(string, "listtrades")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./listtrades", "listtrades", writeFD, readFD, NULL);
			}
		}
		else if(strcmp(string, "leagueshow")==0)
		{	
			int leagueID;
			scanf("%d", &leagueID);
			printf("%d\n", leagueID);
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl("./leagueshow", "leagueshow", writeFD, readFD, leagueID, NULL);
			}
		}/*
		else if(strcmp(string, "teamshow")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}
		else if(strcmp(string, "tradeshow")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}
		else if(strcmp(string, "trade")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}
		else if(strcmp(string, "tradewithdraw")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}
		else if(strcmp(string, "tradeaccept")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}
		else if(strcmp(string, "tradenegociate")==0)
		{
			if(fork())
			{
				wait((int*)0);
			}
			else
			{
				execl();
			}
		}*/
		else 
		{
			printf("invalid command, type 'help' for help\n");
		}
	}
	while(command!=QUIT);
}