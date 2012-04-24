#include "../include/video.h"

tty_t terminals[8];

char videos[7][80*25*2];
int currentTTY=0;


void initScreens()
{
	int i;
	/*for(i=1;i<8; i++)
	{
		terminals[i].video=videos[i-1];
		terminals[i].cursor_x=0;
		terminals[i].cursor_y=0;
		terminals[i].color=WHITE_TXT;
		k_clear_screen(&terminals[i]);
	}*/
	terminals[0].video=VID_DIR;
	terminals[0].color=WHITE_TXT;
	terminals[0].cursor_x=0;
	terminals[0].cursor_y=0;
	k_clear_screen(&terminals[0]);
}

void k_clear_screen(tty_t* tty) 
{
	char *vidmem =  tty->video;
	//char* vidmem = VID_DIR;
	unsigned int i=0;
	while(i < (80*24*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=BLACK_TXT;
		i++;
	};
}


void sys_print(char c)
{
	tty_t* tty;
	/*if(current==-1)
	{
		return;
	}*/
	//tty=process[current].tty;
	tty=&terminals[0];
   // char *video = tty->video;
    char* video= VID_DIR;
    if(c=='\n')
    {
		tty->cursor_y++;
		tty->cursor_x=0;
	}
	else if(c=='\b')
	{
		if(tty->cursor_x!=0)
		{
			tty->cursor_x--;
			video[cursor_index(tty)*2]=' ';
		 }
	}
	else if(c=='\r')
	{
		tty->cursor_x=0;
	}
    else 
    {
		video[cursor_index(tty)*2]=c;
		video[cursor_index(tty)*2+1]=tty->color;
		tty->cursor_x++;
	}
    
    /* Control del cursor */
    if(tty->cursor_x>=WIDTH)
    {
		tty->cursor_y++;
		tty->cursor_x=0;
	}
	scroll(tty);
	if(video==VID_DIR)
		update_cursor(tty);
}

void sys_setcolor(char c)
{
	process[current].tty->color=c;
}

void scroll(tty_t* tty)
{
	if(tty->cursor_y==24){
		/* Scroll down*/
		char *video = tty->video;
		int i;
		for(i=0;i<23*160;i++){
			video[i]=video[i+160];
		}
		while(i<24*160){
			video[i++]=' ';
			video[i++]=WHITE_TXT;
		}
		tty->cursor_y--;
	}	
}


int cursor_index(tty_t* tty)
{
	return (tty->cursor_y * WIDTH + tty->cursor_x);
}

void update_cursor(tty_t* tty)
{
    int index;
	index= cursor_index(tty);
    _IO_out(0x3D4, 14);
    _IO_out(0x3D5, index >> 8);
    _IO_out(0x3D4, 15);
    _IO_out(0x3D5, index);
}

void swapTTY(int num)
{
	if(currentTTY!=num)
	{
		char auxv[160*25];
		memcpy(auxv, VID_DIR, 160*25);
		memcpy(VID_DIR, terminals[num].video, 160*25);
		memcpy(terminals[num].video, auxv, 160*25);
		terminals[currentTTY].video=terminals[num].video;
		terminals[num].video=VID_DIR;
		currentTTY=num;
		update_cursor(&terminals[currentTTY]);
	}
}

