#include "../include/msg.h"

msg_t msgs[MAXMSG];
int msgTail = 0;
int msgHead = 0;


int msgRead(msg_t * info){

	while(msgHead==msgTail){
		msgRBlock();	
	} 
	*info = msgs[msgTail];
	if(msgTail+1>MAXMSG){
		msgTail = 0;
	} else{
		msgTail++;
	}
	printf("%d\n", info->command);
	switch(info->command){
		case 0:
			printf("mkdir\n");
			_mkdir( (char*)info->argv[0] );
		break;
		case 1:
			printf("ls\n");
			_ls((char*)info->argv[0],(char(*)[MAXNAME]) info->argv[1]);
		break;
		case 2:
			printf("format\n");
			initializeFS();
		break;
	}
	msgWAwake();

	return 0;
}

int msgWrite(msg_t * toWrite){
	toWrite->pid = current;

	while(msgHead==(msgTail-1) || (msgTail==0 && msgHead==(MAXMSG-1))) {
		msgWBlock();
	}

	msgs[msgHead] = *toWrite;
	if(msgHead+1>MAXMSG){
		msgHead = 0;
	} else{
		msgHead++;
	}
	_Cli();
	if(process[driverPid].status==BLOCK && process[driverPid].msg){
		msgRAwake();
	}
	_Sti();
	msgWBlock();
	return 0;
}