#include "../common.h"
#include "../msg.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args)
{
    printf("entre al fork\n");
    int msg = atoi(args[4]);
    int code=msg;
    int id=atoi(args[3]);
    void* readFD, *writeFD;
	readFD=(void*)(args[2]);
	writeFD=(void*)(args[1]);
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
