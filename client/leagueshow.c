#include "../common.h"
#include "../msg.h"
#include <stdio.h>

int main(int argc, char** args)
{
	int msg = LEAGUE_SHOW;
	int readFD, writeFD;
	readFD=(int)args[2];
	writeFD=(int) args[1];
    int leagueID=(int)args[3];
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    sndMsg(writeFD, (void*)&leagueID, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    if(msg==LEAGUE_ID_INVALID)
    {
	 printf("invalid league id\n");
	 exit(0);
    }
    while(msg==LEAGUE_SHOW&&msg!=END_LEAGUE_SHOW)
    {
      char string[50];
      rcvString(readFD, string);
      printf("%s", string);
          rcvMsg(readFD, (void*)&msg, sizeof(int));

    }
    
      exit(0);
}