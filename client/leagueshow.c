#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  void* channel;
  clientID=atoi(args[1]);
  connectClient(clientID,&channel);
  int ID=atoi(args[2]);
  int control=atoi(arg[3]);
  int end=atoi(arg[4]);
  int msg=control;
  sndMsg(channel, (void*)&msg, sizeof(int));
  sndMsg(channel, (void*)&ID, sizeof(int));
  rcvMsg(channel, (void*)&msg, sizeof(int));
  if(msg==ID_INVALID)
  {
	   printf("invalid ID\n");
	   exit(0);
  }
  while(msg==control && msg!=end)
  {
     char string[50];
     rcvString(channel, string);
     printf("%s", string);
     rcvMsg(channel, (void*)&msg, sizeof(int));
  }

 exit(0);
}
