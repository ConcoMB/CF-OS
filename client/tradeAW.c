#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    printf("entre al fork\n");
    int msg = atoi(args[3]);
    int code=msg;
    int id=atoi(args[2]), clientID;
    void* readFD, *writeFD;
	clientID=atoi(args[1]);
	connectClient(clientID,&writeFD,&readFD);
    sndMsg(writeFD, (void*)&msg, sizeof(int));
    sndMsg(writeFD, (void*)&id, sizeof(int));
    rcvMsg(readFD, (void*)&msg, sizeof(int));
    if(msg==code)
    {
        printf("Transaction successfull\n");
    }
    else if(msg== ERROR || msg==ID_INVALID)
    {
        printf("An error ocurred, please check the parameters \n");
    }
    exit(0);
}
