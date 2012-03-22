#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = LEAGUE_SHOW;
	int read, write;
	read=(int)args[2];
	write=(int) args[1];
    int leagueID=(int)args[3];
    sndMsg(write, (void*)&msg, sizeof(int));
    sndMsg(write, (void*)&leagueID, sizeof(int));
    rcvMsg(read, (void*)&msg, sizeof(int));
    if(msg==LEAGUE_ID_INVALID)
    {
	 printf("invalid league id\n");
	 exit(0);
    }
    while(msg==LEAGUE_SHOW&&msg!=END_LEAGUE_SHOW)
    {
      char string[50];
      rcvString(read, string);
      printf("%s", string);
          rcvMsg(read, (void*)&msg, sizeof(int));

    }
    
      exit(0);
}