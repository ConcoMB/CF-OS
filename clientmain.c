#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int MsgID;
void startUp();

int main()
{
	sndMsg(DEFAULTID, (void*)NEWCLIENT, sizeof(int));
	rcvMsg(DEFAULTID, (void*)&MsgID, sizeof(int));
	startUp();
}

void startUp()
{
	while(1)
	{
		int handshake;
		char command[10], name[NAME_LENGTH], password[NAME_LENGTH];
		scanf("%s", command);
		if(strcmp(command, "login")==0)
		{

			printf("name:\n");
			scanf("%s", name);
			printf("password:\n");
			scanf("%s", password);
			sndMsg(MsgID, (void*)LOGIN, sizeof(int));
			sndMsg(MsgID, (void*)name, NAME_LENGTH);
			sndMsg(MsgID, (void*)password, NAME_LENGTH);
			rcvMsg(MsgID, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case INCORRECT_PASSWORD:
					printf("incorrect password\n");
					break;
				case USER_NOT_FOUND:
					printf("user unknown\n");
					break;
				default:
					//empieza
			}
		}
		else if(strcmp(command, "signup")==0)
		{
			printf("Enter new name:\n");
			scanf("%s", name);
			printf("password:\n");
			scanf("%s", password);
			sndMsg(MsgID, (void*)SIGNUP, sizeof(int));
			sndMsg(MsgID, (void*)name, NAME_LENGTH);
			sndMsg(MsgID, (void*)password, NAME_LENGTH);
			rcvMsg(MsgID, (void*)&handshake, sizeof(int));
			switch(handshake)
			{
				case USER_NAME_OCCUPIED:
					printf("User name already taken, choose an other\n");
				case NAME_OR_PASSWORD_TOO_LARGE:
					printf("The length of the user name and the password must be lower than 15 characters\n");
				default:
					//empieza
			}
		}
		else
		{
			printf("invalid command\n");
		}
	}
}