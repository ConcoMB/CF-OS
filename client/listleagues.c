#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
    printf("entre al fork\n");
    int msg = SEND_LEAGUE;
    char name[NAME_LENGTH];
    int id;
    int read, write;
	read=(int)args[2];
	write=(int) args[1];
    sndMsg(write, (void*)&msg, sizeof(int));
    rcvMsg(read, (void*)&msg, sizeof(int));
    while(msg!=END_SEND_LEAGUE && msg==SEND_LEAGUE)
    {
	   rcvString(read, name);
	   printf("Liga %s", name);
	   rcvMsg(read, (void*)&id, sizeof(int));
	   printf("-> ID: %d\n",id);
	   rcvMsg(read, (void*)&msg, sizeof(int));
    }
    
    exit(0);
}
    