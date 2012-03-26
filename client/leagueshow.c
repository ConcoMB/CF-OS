#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  void* readFD, *writeFD;
  clientID=atoi(args[1]);
  connectClient(clientID,&writeFD,&readFD);
  int ID=atoi(args[2]);
  int control=atoi(arg[3]);
  int end=atoi(arg[4]);
  int msg=control;
  sndMsg(writeFD, (void*)&msg, sizeof(int));
  sndMsg(writeFD, (void*)&ID, sizeof(int));
  rcvMsg(readFD, (void*)&msg, sizeof(int));
  if(msg==ID_INVALID)
  {
	   printf("invalid ID\n");
	   exit(0);
  }
  while(msg==control && msg!=end)
  {
     char string[50];
     rcvString(readFD, string);
     printf("%s", string);
     rcvMsg(readFD, (void*)&msg, sizeof(int));
  }
   
 exit(0);
}
