/* SHELL */

#include "../include/shell.h"


int shell(int argc, char** argv){
	char c;
	int background=0;
	char buffer[MAX_CMD_SIZE+1];
	char last_cmd[CMD_MEMORY][MAX_CMD_SIZE+1];
	int i,mem, nothing;
	char shell_color=0x09;
	char user_color=0x07;
	
	for(mem=0;mem<CMD_MEMORY;mem++){
		last_cmd[mem][0]=0;
	}													
	while(1){
		background=0;
		nothing=0;
		__setcolor(&shell_color);
		printf("Shell->: ");
		__setcolor(&user_color);
		i=0;
		mem=-1;
		do{
			//sleep(500);
			c=getchar();
			if(c=='\b'){
				if(i>0){
					i--;
					putchar(c);
				}
			}
			else if(c=='\x11'){
				/*UP*/
				if(mem<CMD_MEMORY-1){
					mem++;
					int j;
					for(j=0;j<i;j++){
						putchar('\b');
					}
					i=strlen(last_cmd[mem]);
					strcpy(buffer,last_cmd[mem]);
					printf("%s",buffer);
				}
			}
			else if(c=='\x13'){
				/*DOWN*/
				if(mem>0){
					mem--;
					int j;
					for(j=0;j<i;j++){
						putchar('\b');
					}
					i=strlen(last_cmd[mem]);
					strcpy(buffer,last_cmd[mem]);
					printf("%s",buffer);
				}
			}
			else if(c=='\x12'){
				//do nothing
			}
			else{
				if(i<MAX_CMD_SIZE||c=='\n'){
				  putchar(c);
				  buffer[i]=c;
				  i++;
				}
			}
		}while(c!='\n');
		buffer[i-1]=0;
		/*GUARDAR COMANDOS ANTERIORES*/
		int k;
		for(k=CMD_MEMORY-2;k>=0;k--){
			strcpy(last_cmd[k+1],last_cmd[k]);
		}
		strcpy(last_cmd[0],buffer);
		
		int len=strlen(buffer);
		int (*func)(int, char**);
		int argc;
		char* argv[4];
		if(len==0)
		{
			/*VACIO*/
		}
		if(buffer[len-1]=='&')
		{
			background=1;
		}
		if(substr("who",buffer)){
			func=who;
			argc=1;
			argv[0]="who";
			argv[1]=(char*)0;
		}
		else if(substr("echo ", buffer)){
			func=echo;
			argc=2;
			argv[0]="echo";
			argv[1]=buffer+5;
			argv[2]=(char*)0;
		}
		else if(substr("color ", buffer)){
		    func=chColor;
		    argc=3;
		    argv[0]="color";
		    argv[1]=buffer+6;
		    argv[2]=(char*)&user_color;
		    argv[3]=(char*)0;
		}
		else if(substr("time",buffer)){
			func=time;
			argc=1;
			argv[0]="time";
			argv[1]=(char*)0;
		}	
		else if(substr("keyboard ", buffer)){
			func=keyboard;
			argc=2;
			argv[0]="keyboard";
			argv[1]=buffer+9;
			argv[2]=(char*)0;
		}
		
		else if(substr("lostquote", buffer)){
			func=lost;
			argc=1;
			argv[0]="lostquote";
			argv[1]=(char*)0;
		}
		else if(substr("mastersword", buffer)){
			func=mastersword;
			argc=1;
			argv[0]="mastersword";
			argv[1]=(char*)0;
		}
		else if(substr("mario", buffer)){
			func=mario;
			argc=1;
			argv[0]="mario";
			argv[1]=(char*)0;
		}
		else if(substr("help", buffer)){
			func=help;
			argc=1;
			argv[0]="help";
			argv[1]=(char*)0;	
		}
		else if(substr("kill ", buffer)){
			func=Kill;
			argc=2;
			argv[0]="kill";
			argv[1]=buffer+5;
			argv[2]=(char*)0;
		}
		else if(substr("top", buffer)){
			func=top;
			argc=1;
			argv[1]=(char*)0;	
			argv[0]="top";
		}
		else if(substr("multiply3", buffer)){
			func=print3;
			argc=1;
			argv[0]="multiply 3";
			argv[1]=(char*)0;
		}
		else if(substr("malloc ", buffer)){
			func=Malloc;
			argc=2;
			argv[0]="malloc";
			argv[1]=buffer+7;
			argv[2]=(char*)0;
		} else if(substr("free ", buffer)){
			func=Free;
			argc=2;
			argv[0]="free";
			argv[1]=buffer+5;
			argv[2]=(char*)0;
		}
		else if(strcmp("format", buffer)==0){
			func=format;
			argc=1;
			argv[0]="format";
			argv[1]=(char*)0;
		}
		else if(substr("mkdir ", buffer)){
			func=mkdir;
			argc=2;
			argv[0]="mkdir";
			argv[2]=(char*)0;
			argv[1]=buffer+6;
		}
		else if(substr("cat ", buffer)){
			func=cat;
			argc=2;
			argv[0]="cat";
			argv[2]=(char*)0;
			argv[1]=buffer+4;
		}
		else if(substr("touch ", buffer)){
			func=touch;
			argc=2;
			argv[0]="touch";
			argv[2]=(char*)0;
			argv[1]=buffer+6;
		}
		else if(substr("cd ", buffer)){
			func=cd;
			argc=2;
			argv[0]="cd";
			argv[2]=(char*)0;
			argv[1]=buffer+3;
		
		}
		else if(substr("attach ", buffer)){
			func=attach;
			argc=2;
			argv[0]="attach";
			argv[2]=(char*)0;
			argv[1]=buffer+7;
		}
		else if(substr("ln ", buffer)){
			func=ln;
			argc=2;
			argv[0]="ln";
			argv[3]=(char*)0;
			int i;
			for(i=3;buffer[i]!=' '&&buffer[i];i++);
			buffer[i]=0;
			argv[2]=buffer+i+1;
			argv[1]=buffer+3;
			if(strlen(argv[1])==0||strlen(argv[2])==0){
				printf("Invalid arguments.\n");
			}
		}
		else if(substr("ls", buffer)){
			func=ls;
			argc=2;
			argv[0]="ls";
			if(buffer[2] == ' '){
				argv[1]=buffer+3;
			}else{
				argv[1]=".";
			}
			argv[2]=(char*)0;			
		}else{
			nothing=1;
			printf("Command not found\n");
		}
		if(!nothing)
		{
			if(background)
			{
				__createChild(func, argc,argv);
			}
			else
			{
				func(argc,argv);
			}
		}
	}
}

