/* SHELL */

#include "../include/shell.h"

char color(char* color_name){
	if(strcmp("blue",color_name)==0){
		return 0x01;
	}else if(strcmp("green",color_name)==0){
		return 0x02;
	}else if(strcmp("cyan",color_name)==0){
		return 0x03;
	}else if(strcmp("red",color_name)==0){
		return 0x04;
	}else if(strcmp("magenta",color_name)==0){
		return 0x05;
	}else if(strcmp("brown",color_name)==0){
		return 0x06;
	}else if(strcmp("light gray",color_name)==0){
		return 0x07;
	}else if(strcmp("gray",color_name)==0){
		return 0x08;
	}else if(strcmp("light blue",color_name)==0){
		return 0x09;
	}else if(strcmp("light green",color_name)==0){
		return 0x0A;
	}else if(strcmp("light cyan",color_name)==0){
		return 0x0B;
	}else if(strcmp("orange",color_name)==0){
		return 0x0C;
	}else if(strcmp("pink",color_name)==0){
		return 0x0D;
	}else if(strcmp("yellow",color_name)==0){
		return 0x0E;
	}else if(strcmp("white",color_name)==0){
		return 0x0F;
	}else{
		return 0;
	}
}

void set_out_stream_command(char* c){
	if(strcmp(c, "screen")==0){
		set_out_stream(SCREEN);
	}else if(strcmp(c, "speaker")==0){
		set_out_stream(SPEAKER);
	}else{
		printf("invalid out stream\n");
	}
}

void print_memory(){
	int s = stack_count();
	int h = heap_count();
	printf("\nSTACK:\n %d bytes used\n", s);
	printf("\n HEAP:\n %d pages used -> %d bytes\n\n", h, h*4096);
}

int shell(int argc, char** argv){
	char c;
	int background=0;
	char buffer[MAX_CMD_SIZE+1];
	char last_cmd[CMD_MEMORY][MAX_CMD_SIZE+1];
	int i,mem;
	char shell_color=0x09;
	char user_color=0x07;
	
	for(mem=0;mem<CMD_MEMORY;mem++){
		last_cmd[mem][0]=0;
	}													
	while(1){
		background=0;
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
		
		if(len==0)
		{
			/*VACIO*/
		}
		if(buffer[len-1]=='&')
		{
			background=1;
		}
		if(strcmp("who",buffer)==0){
			printf("\n************\nT Mehdi\nF Ramundo\nC Mader Blanco\n************\n\n");
		}
		else if(substr("echo ", buffer)){
			printf("%s\n",buffer+5);
		}
		else if(substr("color ", buffer)){
		  char tmp=color(buffer+6);
		  if(tmp==0){
		    printf("Invalid color name\n");
		  }
		  else{
		    user_color=tmp;
		  }
		}
		else if(strcmp("time",buffer)==0){
			int m, h;
			m=getmin();
			h=gethour();
			if(m<10){
				printf("%d:0%d\n",h,m);
			}else{
				printf("%d:%d\n",h,m);
			}
		}	
		else if(substr("keyboard ", buffer)){
			if(strcmp("ESP", buffer+9)==0){
				set_scancode(1);
			}else if(strcmp("ENG", buffer+9)==0){
				set_scancode(2);
			}else{
				printf("Unsuported layout\n");
			}
		}
		else if(substr("speak ", buffer)){
			speak(buffer+6);
		}
		else if(strcmp("memstat", buffer)==0){
			print_memory();
		}
		else if(substr("memalloc ", buffer)){
			i=atoi(buffer+9);
			if(malloc(i)!=0){
				printf("Memory allocated\n");
			}else{
				printf("Not enough memory\n");
			}
		}
		else if(substr("memcalloc ", buffer)){
			i=atoi(buffer+10);
			if(calloc(i)!=0){
				printf("Memory allocated\n");
			}else{
				printf("Not enough memory\n");
			}
		}
		else if(substr("pageprint ", buffer)){
			i=atoi(buffer+10);
			if(pageprint(i)==0){
				printf("Invalid argument");
			}
		}
		else if(substr("memfree ", buffer)){
			i=atoi(buffer+8);
			if(free((void*)((i+530)*4096))){
				printf("Memory freed\n");
			}
			else{
				printf("Invalid argument\n");
			}
		}
		else if(strcmp("lostquote", buffer)==0){
			printf("%s", get_quote());
		}
		else if(strcmp("mastersword", buffer)==0){
			print_zelda();
		}
		else if(strcmp("mario", buffer)==0){
			print_mario();
		}
		else if(strcmp("help", buffer)==0){
			help();
		}
		else if(substr("kill ", buffer)){
			i=atoi(buffer+5);
			switch(kill(i)){
				case 0:
					printf("Procces killed succefully\n");
					break;
				case 1:
					printf("PID invalid\n");
					break;
				case 2:
					printf("Already free\n");
					break;
			}
		}
		else if(strcmp("top", buffer)==0){
			top();
		}
		else{
			__createChild(shell,0,0);
			printf("Command not found\n");
		}
		
	}
}

void help(){
  printf("/*** COMMAND LIST ***/\n\n"
	 " -top (show the current running process)\n"
	 " -kill <procesid> (kill the process)\n"
	 " -time  (prints the localtime)\n"
	 " -who  (info about the developers)\n"
	 " -echo <message>  (prints message)\n"
	 " -speak <message>  (outputs the message through the PC speaker)\n"
	 " -color <color>  (changes the font color to the specified one)\n"
	 "       possible colors: red, blue, green, yellow, white, pink, orange, ...\n"
	 " -keyboard <layout>  (changes the keyboard layout to the specified one)\n"
	 "       possible layouts: ESP, ENG\n"
	 " -memalloc <bytes>  (allocates pages to contain as many bytes as specified)\n"
	 " -memcalloc <bytes>  (similar to memalloc, inits the memory with zeros)\n"
	 " -memfree <page_n>  (frees the user page specified if allocated)\n"
	 " -memstat  (prints info about memory status: stack size and heap size)\n"
	 " -pageprint <page_n>  (prints the first 16 bytes of the allocated user page)\n"
	 " -lostquote  (prints a random quote from the famous TV series LOST)\n"
	 " -mario  (displays the face of Mario)\n"
	 " -mastersword  (displays Link's Sword)\n"
	 " -mastermind  (launches Mastermind game)\n\n"
	 "/** For more information please refer to the User Manual **/\n");
}

