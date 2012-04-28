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
		char* argv[3];
		if(len==0)
		{
			/*VACIO*/
		}
		if(buffer[len-1]=='&')
		{
			background=1;
		}
		if(strcmp("who",buffer)==0){
			func=who;
			argc=0;
			argv[0]=(char*)0;
		}
		else if(substr("echo ", buffer)){
			func=echo;
			argc=1;
			argv[0]=buffer+5;
			argv[2]=(char*)0;
		}
		else if(substr("color ", buffer)){
		    func=chColor;
		    argc=2;
		    argv[0]=buffer+6;
		    argv[1]=(char*)&user_color;
		    argv[2]=(char*)0;
		}
		else if(strcmp("time",buffer)==0){
			func=time;
			argc=0;
			argv[0]=(char*)0;
		}	
		else if(substr("keyboard ", buffer)){
			func=keyboard;
			argc=1;
			argv[0]=buffer+9;
			argv[1]=(char*)0;
		}
		
		else if(strcmp("lostquote", buffer)==0){
			func=lost;
			argc=0;
			argv[0]=(char*)0;
		}
		else if(strcmp("mastersword", buffer)==0){
			func=mastersword;
			argc=0;
			argv[0]=(char*)0;
		}
		else if(strcmp("mario", buffer)==0){
			func=mario;
			argc=0;
			argv[0]=(char*)0;
		}
		else if(strcmp("help", buffer)==0){
			func=help;
			argc=0;
			argv[0]=(char*)0;	
		}
		else if(substr("kill ", buffer)){
			func=Kill;
			argc=1;
			argv[1]=(char*)0;
			argv[0]=buffer+5;
		}
		else if(strcmp("top", buffer)==0){
			func=top;
			argc=0;
			argv[0]=(char*)0;	
		}
		else{
			nothing=1;
			printf("Command not found\n");
		}
		if(!nothing)
		{
			if(background)
			{
				
			}else{
				func(argc,argv);
			}
		}
	}
}

