#include "../include/commands.h"

int who(int argc, char** argv)
{
	printf("\n************\nT Mehdi\nF Ramundo\nC Mader Blanco\n************\n\n");
	while(1);
	return 0;
}

int echo(int argc, char** argv)
{
	printf("%s\n",argv[1]);
	return 0;
}

int chColor(int argc, char** argv)
{
	char tmp= color(argv[1]);
	if(tmp==0)
	{
		printf("Invalid color name\n");
	}
	else
	{
		*(argv[2])=tmp;
	}
	return 0;	
}

char color(char* color_name)
{
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

int time(int argc, char** argv)
{
	int m, h;
	m=getmin();
	h=gethour();
	if(m<10){
		printf("%d:0%d\n",h,m);
	}else{
		printf("%d:%d\n",h,m);
	}
	return 0;
}

int keyboard(int argc, char** argv)
{
	if(strcmp("ESP", argv[1])==0){
		set_scancode(1);
	}else if(strcmp("ENG", argv[1])==0){
		set_scancode(2);
	}else{
		printf("Unsuported layout\n");
	}
	return 0;
}

int lost(int argc, char** argv)
{
	printf("%s", get_quote());
	return 0;
}

int mastersword(int argc, char** argv)
{
	print_zelda();
	return 0;
}

int mario(int argc, char** argv)
{
	print_mario();
	return 0;
}

int help(int argc, char** argv)
{
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
	return 0;
}

int Kill(int argc, char** argv)
{
	_Cli();

	switch(kill(atoi(argv[1])))
	{
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
	_Sti();
	return 0;
}

int Malloc(int argc, char** argv){
	int bytes = atoi(argv[1]);
	void* mem = malloc(bytes);
	printf("memory allocated at: %d\n", (int)mem);
	return 0;
}

int Free(int argc, char** argv)
{
	int mem = atoi(argv[1]);
	free((void*)mem);
	printf("memory freed\n");
	return 0;
}


int top(int argc, char** argv)
{
	int i=0;
	topInfo_t ti;
	while(1)
	{
		clearScreen();
		resetCursor();
		i=0;
		__top(&ti);
		printf("==========================TOP===============================\n\n"
		  "   PID         %%CPU      memory pages       status        process name\n-----------------------------------------------------------------\n");
		for(;i<ti.cant; i++)
		{
			int perc=ti.percent[i];
			char * status;
			switch(ti.stats[i]){
				case RUN:
					status="Running";
					break;
				case READY:
					status="Ready  ";
					break;
				case BLOCK:
					status="Blocked";
					break;
				default:
					status="-";
			}
			if(perc<10){
				printf("   %d            0%d             %d           %s          %s\n", ti.pids[i],perc, ti.mem[i], status, ti.names[i]);
			}else{
				printf("   %d            %d             %d           %s         %s\n", ti.pids[i],perc, ti.mem[i], status,ti.names[i]);
			}
		}
		printf("\n");
		i=0;
		while(i<500000)
		{
			if(getCharNB()=='q')
			{
				return 0;
			}
			i++;
		}
		//sleep(2000);
	}
	return 0;
}
