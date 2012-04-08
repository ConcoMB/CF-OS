#include "client.h"

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
	int loged=0;
	char name[NAME_LENGTH];
	while(!loged)
	{
		char command[10];
		printf(BLUE"Type 'login' or 'signup': "WHITE);
		scanf("%s", command);
		if(strcmp(command, "login")==0)
		{
			loged=logClient(name);
		}
		else if(strcmp(command, "signup")==0)
		{
			loged=signClient(name);
		}
		else
		{
			printf(RED"Invalid command\n"WHITE);
		}
	}
	shell(msgID,name);
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


int logClient(char* name)
{
	char password[NAME_LENGTH];
	int handshake;
	int aux=LOGIN;
	sndMsg(channel, (void*)&aux, sizeof(int));
	printf(CYAN"name: "WHITE);
	fflush(stdout);
	scanf("%s", name);
	sndString(channel, name);
	printf(CYAN"password: "WHITE);
	fflush(stdout);
	scanf("%s", password);
	sndString(channel,password);
	rcvMsg(channel, (void*)&handshake, sizeof(int));
	switch(handshake)
	{
		case INCORRECT_PASSWORD:
			printf(RED"Incorrect password\n"WHITE);
			handshake=0;
			break;
		case USER_NOT_FOUND:
			handshake=0;			
			printf(RED"User unknown\n"WHITE);
			break;
		default:
			handshake= 1;
	}
	return handshake;
}
	
	
int signClient(char * name)
{
	char password[NAME_LENGTH];
	int handshake;
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
			handshake=0;
			break;
		case NAME_OR_PASSWORD_TOO_LARGE:
			printf(RED"The length of the user name and the password must be lower than 15 characters\n"WHITE);
			handshake=0;
			break;
		default:
			handshake= 1;
	}
	return handshake;
}
