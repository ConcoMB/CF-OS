#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "joining.h"
#include "league.h"

int main(int argc, char** argv)
{
	printf("me forkie\n");
	int type, aux;
	int id= (int)argv[1][0];
	printf("id %d\n",id );
	char name[NAME_LENGTH], password[NAME_LENGTH];
	
	connect(id);
	rcvMsg(id,(void*)&type, sizeof(int));
	printf("%d\n", type);
	rcvString(id, name);
	printf("%s\n", name);
	rcvString(id, password);
	printf("%s\n", password);
	if(type==LOGIN)
	{
		if((aux=logIn(name,password))!=0)
		{
			
			sndMsg(id, (void*)&aux, sizeof(int));
		}
	}
	else if(type==SIGNUP)
	{
		if((aux=signUp(name, password))!=0)
		{
			//error
			sndMsg(id, (void*)&aux, sizeof(int));
		}
		logIn(name, password);
	}
}