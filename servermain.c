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
		int type;
		char name[NAME_LENGTH], password[NAME_LENGTH];

		rcvMsg(type);
		rcvMsg(name);
		rcvMsg(password);
		if(type==LOGIN)
		{
			if(logIn(name,password)!=0)
			{
				sendMsg(aux);
			}
		}
		else if(type==SIGNUP)
		{
			if(signUp(name, password)!=0)
			{
				sendMsg(aux);
			}
			logIn(name, password);
		}

	}
}