#include "../include/msg.h"

msg_t msgs[MAXMSG];
int msgTail = 0;
int msgCount = 0;
int msgHead = 0;


int msgRead(msg_t * info){

	if(msgCount==0){
		process[driverPid].msg = 1;
		process[driverPid].status = BLOCK;
		_sys_yield();	
	} 
	if(msgCount<MAXMSG){
		*info = msgs[msgTail];
		if(msgTail+1>MAXMSG){
			msgTail = 0;
		} else{
			msgTail++;
		}

		switch(info->command){
			case 0:
				_mkdir( (char*)info->argv[0] );
			break;
			case 1:
				//_ls((char*)info->argv[0],(char[][MAXNAME]) info->argv[1]);
			break;
		}
	}
	return 0;
}

int msgWrite(msg_t * toWrite){
	toWrite->pid = current;
	msgs[msgHead] = *toWrite;
	msgCount++;
	if(msgHead+1>MAXMSG){
		msgHead = 0;
	} else{
		msgHead++;
	}
	if(msgCount>0){
		process[driverPid].msg = 0;
		process[driverPid].status = READY;
	}
	if(msgCount==MAXMSG){

	} else{
		process[current].msg = 1;
		process[current].status = BLOCK;
		_sys_yield();	
	}
	return 0;
}