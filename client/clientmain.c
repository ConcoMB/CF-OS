#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../msg.h"
#include "../common.h"

void startUp(int MsgID);

int main()
{
	int aux= NEWCLIENT, MsgID;
	char defWChannel[3], defRChannel[3];
	sprintf(defWChannel, "%c%d", 'c', DEFAULTID);
	sprintf(defRChannel, "%c%d", 's', DEFAULTID);
	connect(defWChannel);
	connect(defRChannel);

	sndMsg(defWChannel, (void*)&aux, sizeof(int));
	printf("mande\n");
	rcvMsg(defRChannel, (void*)&MsgID, sizeof(int));
	printf("recibi msgid %d\n", MsgID);

		

	startUp(MsgID);
}

void startUp(int MsgID)
{
	char writeChannel[4], readChannel[4];
	sprintf(readChannel, "%c%d", 's', MsgID);
	sprintf(writeChannel, "%c%d", 'c', MsgID);
	connect(readChannel);
	connect(writeChannel);
	while(1)
	{
		int handshake;
		char command[10], name[NAME_LENGTH], password[NAME_LENGTH];
		scanf("%s", command);
		if(strcmp(command, "login")==0)
		{
			int aux=LOGIN;
			sndMsg(writeChannel, (void*)&aux, sizeof(int));
			printf("name:\n");
			scanf("%s", name);
			sndString(writeChannel, name);
			printf("password:\n");
			scanf("%s", password);
			sndString(writeChannel,password);
			rcvMsg(readChannel, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case INCORRECT_PASSWORD:
					printf("incorrect password\n");
					break;
				case USER_NOT_FOUND:
					printf("user unknown\n");
					break;
					//empieza
			}
		}
		else if(strcmp(command, "signup")==0)
		{
			int aux=SIGNUP;
			sndMsg(writeChannel, (void*)&aux, sizeof(int));
			printf("Enter new name:\n");
			scanf("%s", name);
			sndString(writeChannel, name);			
			printf("password:\n");
			scanf("%s", password);
			sndString(writeChannel, password);
			rcvMsg(readChannel, (void*)&handshake, sizeof(int));
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
			}
		}
		else
		{
			printf("invalid command\n");
		}
	}
}