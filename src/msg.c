 #include "../include/msg.h"

msg_t msgs[MAXMSG];
int msgTail = 0;
int msgHead = 0;

static void setContext(int pid){
	_Cli();
	current=pid;
}

static void unsetContext(){
	current=driverPid;
	_Sti();
}

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
	switch(info->command){
		case 0:
			_mkdir( (char*)info->argv[0] );
			break;
		case 1:
			setContext(info->pid);
			_ls((char*)info->argv[0]);
			unsetContext();
			break;
		case 2:
			initializeFS();
			break;
		case 3:
			setContext(info->pid);
			_cat((char*)info->argv[0]);	
			unsetContext();		
			break;
		case 4:
			_touch((char*)info->argv[0]);
			break;
		case 5:
			attach((char*)info->argv[0],(char*)info->argv[1]);
			break;
		case 6:
			_ln((char*)info->argv[0],(char*)info->argv[1]);
			break;
		case 7:
			_rm((char*)info->argv[0],0);
		break;
		case 8:
			_cp((char*)info->argv[0],(char*)info->argv[1]);
			break;
		case 9:
			_mv((char*)info->argv[0],(char*)info->argv[1]);
			break;
		case 10:
			revertLast((char*)info->argv[0]);
			break;
		case 11:
			setContext(info->pid);
			printVersions((char*)info->argv[0]);
			unsetContext();
			break;
		case 12:
			_cd((char*)info->argv[0]);
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
