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
	int ans=0;
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
			setContext(info->pid);
			ans = _mkdir( (char*)info->argv[0] );
			unsetContext();
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
			ans=_cat((char*)info->argv[0]);	
			unsetContext();		
			break;
		case 4:
			setContext(info->pid);			
			ans=_touch((char*)info->argv[0]);
			unsetContext();
			break;
		case 5:
			setContext(info->pid);
			ans=_attach((char*)info->argv[0],(char*)info->argv[1]);
			unsetContext();
			break;
		case 6:
			setContext(info->pid);
			ans = _ln((char*)info->argv[0],(char*)info->argv[1]);
			unsetContext();		
			break;
		case 7:
			setContext(info->pid);
			ans = _rm((char*)info->argv[0],0);
			unsetContext();
			break;
		case 8:
			setContext(info->pid);
			ans=_cp((char*)info->argv[0],(char*)info->argv[1]);
			unsetContext();
			break;
		case 9:
			setContext(info->pid);
			ans = _mv((char*)info->argv[0],(char*)info->argv[1]);
			unsetContext();
			break;
		case 10:
			setContext(info->pid);
			ans=revertLast((char*)info->argv[0]);
			unsetContext();
			break;
		case 11:
			setContext(info->pid);
			ans=printVersions((char*)info->argv[0]);
			unsetContext();
			break;
		case 12:
			setContext(info->pid);
			ans= _cd((char*)info->argv[0]);
			unsetContext();
			break;
		case 13:
			setContext(info->pid);		
			ans=revertTo((char*)info->argv[0], (int)info->argv[1]);
			unsetContext();
			break;
		case 14:
			setContext(info->pid);
			printTree(tree);
			unsetContext();
			break;
		case 15:
			setContext(info->pid);
			printTable();
			unsetContext();
			break;
		case 16:
			setContext(info->pid);
			printBitMap();
			unsetContext();
			break;
		case 17:
			setContext(info->pid);
			ans=bigFile((char*)info->argv[0]);
			unsetContext();
			break;
		case 18:
			setContext(info->pid);
			ans = _rm((char*)info->argv[0],1);
			unsetContext();
			break;
		case 19:
			setContext(info->pid);
			getCWD((char*)info->argv[0]);
			unsetContext();
			break;
		case 20:
			setContext(info->pid);
			_lsr((char*)info->argv[0]);
			unsetContext();
			break;
		case 21:
			setContext(info->pid);
			_readSector((char*)info->argv[0], (int)info->argv[1], (char*)info->argv[2]);
			unsetContext();
			break;
	}
	if(ans!=0){
		setContext(info->pid);
		printError(ans);
		unsetContext();
	}
	ans=0;
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

void printError(int ans){
	switch(ans){
		case -5:
			printf("Name already used in this directory\n");
			break;
		case -2:
			printf("File or directory not found\n");
			break;
		case -9:
			printf("Unable to remove the root\n");
			break;
		case -3:
			printf("Cannot remove directory\n");
			break;
		case -6:
			printf("Cannot move there\n");
			break;
		case -7:
			printf("Cannot copy there\n");
			break;	
		case -11:
			printf("Cannot attach something to a directory\n");
			break;
		case -13:
			printf("Not so many versions\n");
			break;
		case -14:
			printf("Cannot move to a file\n");
			break;
		case -66:
			printf("Full disc, remove some files before trying again\n");
			break;
		case -18:
			printf("Impossible to revert to that version. There's an other file with the same name in that directory now.\n"
			"Remove it before reverting to this version again\n");
			break;
		case -19:
			printf("Cannot create a file into a file\n");
			break;
		case -20:
			printf("Cannot remove CWD\n");
			break;									
	}
}
