#include "../include/pipe.h"

msg_t msgs[MAXMSG];
int readIndex = 0;
int msgCount = 0;
int writeIndex = 0;


int read(msg_t * ans){
	while(msgCount == 0);
	ans = msgs[readIndex];
	if(readIndex+1>MAXMSG){
		readIndex = 0;
	} else{
		readIndex++;
	}
	if(msgCount==0){
		readBlock = TRUE;
	}
	if(msgCount<MAXMSG){
		writeBlock = FALSE;
	}
}

int write(msg_t * toWrite){
	msgs[writeIndex] = toWrite;
	msgCount++;
	if(writeIndex+1>MAXMSG){
		writeIndex = 0;
	} else{
		writeIndex++;
	}
	if(msgCount>0){
		readBlock=FALSE;
	}
	if(msgCount==MAXMSG){
		writeBlock=TRUE;
	}
}