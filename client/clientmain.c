#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "../msg.h"
#include "../common.h"
#include "connection.h"
#include "shell.h"

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
	shell(msgID);
}

void sighandler(int sig)
{
    disconnect(channel);
    exit(0);
}
