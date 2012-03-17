#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "server.h"
#include "league.h"


int main()
{
	pthread_t thread;


	return 0;
}

void * listenClient()
{
	while(1)
	{
		int msg;
		rcvMsg(DEFAULTID, (void*)&msg, sizeof(int));
		

		int type;
		char name[NAME_LENGTH], password[NAME_LENGTH];

		rcvMsg(id,(void*)&type, sizeof(int));
		rcvMsg(id, (void*)name, NAME_LENGTH);
		rcvMsg(id, (void*)password, NAME_LENGTH);
		if(type==LOGIN)
		{
			if(logIn(name,password)!=0)
			{
				sendMsg(id, (void*)&aux, sizeof(int));
			}
		}
		else if(type==SIGNUP)
		{
			if(signUp(name, password)!=0)
			{
				sendMsg(id, (void*)&aux, sizeof(int));
			}
			logIn(name, password);
		}

	}
}