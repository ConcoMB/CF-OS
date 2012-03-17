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
		if(mgs==NEWCLIENT)
		{
			if(fork()==0)
			{
				exec("./clientAttendant");
			}
		}
	}
}

		

	