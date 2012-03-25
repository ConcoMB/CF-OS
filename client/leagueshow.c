#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** args)
{
  printf("entre\n");
  void* readFD, *writeFD;
	readFD=(void*)(args[2]);
	writeFD=(void*)(args[1]);
  int ID=atoi(args[3]);
  int control=atoi(arg[4]);
  int end=atoi(arg[5]);
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
