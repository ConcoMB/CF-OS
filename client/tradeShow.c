#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  int msg, clientID;
  void* readFD, *writeFD;
  clientID=atoi(args[1]);
  connectClient(clientID,&writeFD,&readFD);
  int ID=atoi(args[2]);
  msg=TRADE_SHOW;
  char string[50];
  sndMsg(writeFD, (void*)&msg, sizeof(int));
  sndMsg(writeFD, (void*)&ID, sizeof(int));
  rcvMsg(readFD, (void*)&msg, sizeof(int));
  if(msg==ID_INVALID)
  {
  	printf("ID INVALID\n");
  }
  else
  {
  	rcvString(readFD, string);
  	printf("%s\n", string);
  }
  exit(0);
}
