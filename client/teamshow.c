#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre al leagueshow\n");
  int msg = TEAM_SHOW;
  int readFD, writeFD;
  readFD=atoi(args[2]);
  writeFD=atoi(args[1]);
  int teamID=atoi(args[3]);
  sndMsg(writeFD, (void*)&msg, sizeof(int));
  sndMsg(writeFD, (void*)&teamID, sizeof(int));
  rcvMsg(readFD, (void*)&msg, sizeof(int));