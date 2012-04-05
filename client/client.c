#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "../msg.h"
#include "../common.h"
#include "connection.h"
#include "shell.h"
#include "../colors.h"

void sighandler(int sig);
void userLog();
void start();
void rejoinDraft(int msgID);
void makeDefConnection();
void* channel;
void* defChannel;
int msgID;
void* keepAlive(void* arg);

int main()
{
	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
	makeDefConnection(&msgID);
	connectClient(msgID,&channel);
	pthread_t keepAliveThread;
	pthread_create(&keepAliveThread, NULL, keepAlive, NULL);
	while(1)
	{
		userLog();
	}
}


void makeDefConnection()
{
	int aux= NEWCLIENT;
	defChannel=connectChannel(DEFAULTID+1);
	sndMsg(defChannel, (void*)&aux, sizeof(int));
	printf(GREEN "Connecting to server...");
	rcvMsg(defChannel, (void*)&msgID, sizeof(int));
	printf("OK\n" WHITE);
}

void userLog()
{
	int handshake;
	int loged=0;
	while(!loged)
	{
		char command[10], name[NAME_LENGTH], password[NAME_LENGTH];
		printf(BLUE"Type 'login' or 'signup': "WHITE);
		scanf("%s", command);
		if(strcmp(command, "login")==0)
		{
			int aux=LOGIN;
			sndMsg(channel, (void*)&aux, sizeof(int));
			printf(CYAN"name: "WHITE);
			scanf("%s", name);
			sndString(channel, name);
			printf(CYAN"password: "WHITE);
			scanf("%s", password);
			sndString(channel,password);
			rcvMsg(channel, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case INCORRECT_PASSWORD:
					printf(RED"Incorrect password\n"WHITE);
					break;
				case USER_NOT_FOUND:
					printf(RED"User unknown\n"WHITE);
					break;
				default:
					loged=1;
			}
		}
		else if(strcmp(command, "signup")==0)
		{
			int aux=SIGNUP;
			sndMsg(channel, (void*)&aux, sizeof(int));
			printf(GREEN"Enter new name: "WHITE);
			scanf("%s", name);
			sndString(channel, name);
			printf(GREEN"Password: "WHITE);
			scanf("%s", password);
			sndString(channel, password);
			rcvMsg(channel, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case NAME_OCCUPIED:
					printf(RED"User name already taken, choose an other\n"WHITE);
					break;
				case NAME_OR_PASSWORD_TOO_LARGE:
					printf(RED"The length of the user name and the password must be lower than 15 characters\n"WHITE);
					break;
				default:
					loged=1;
			}
		}
		else
		{
			printf(RED"Invalid command\n"WHITE);
		}
	}
	/*rcvMsg(channel, (void*)&handshake, sizeof(int));
	if(handshake==USER_DRAFTING)
	{
		rejoinDraft(msgID);
	}*/
	shell(msgID);
}

void rejoinDraft(msgID)
{
	int aux;
	char auxString[NAME_LENGTH], idStr[5], *args[4], *path;
	rcvMsg(channel, (void*)&aux, sizeof(int));
	sprintf(auxString, "%d",aux);
	sprintf(idStr,"%d",msgID);
	path="./draft.e";
	args[0]="draft.e";
	args[1]=idStr;
	args[2]=auxString;
	args[3]=NULL;
	if(fork())
	{
		wait((int*) 0);
	}
	else
	{
		execv(path, args);
	}
}	

void sighandler(int sig)
{
	int msg=CLIENT_DISCONNECT-msgID;
	sndMsg(defChannel,&msg,sizeof(int));
	disconnect(defChannel);
    disconnect(channel);
    exit(0);
}

void* keepAlive(void* arg)
{
	while(1)
	{
		int msg=CLIENT_ALIVE+msgID;
		sndMsg(defChannel,&msg,sizeof(int));
		sleep(10);
	}
}
