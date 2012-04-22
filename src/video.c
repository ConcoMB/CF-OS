#include "../include/video.h"

tty_t tty[8];
char videos[7][80*25*2];
int currentTTY=0;


void initScreens()
{
	int i;
	for(i=1;i<8; i++)
	{
		tty[i].video=videos[i-1];

	}
	tty[0].video=VID_DIR;
}

void k_clear_screen() 
{
	char *vidmem =  process[current].tty.video;
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
	tty_t tty=process[current].tty;
    char *video = tty.video;
    if(c=='\n')
    {
		tty.cursor_y++;
		tty.cursor_x=0;
	}
	else if(c=='\b')
	{
		if(tty.cursor_x!=0)
		{
			tty.cursor_x--;
			video[cursor_index()*2]=' ';
		 }
	}
	else if(c=='\r')
	{
		tty.cursor_x=0;
	}
    else 
    {
		video[cursor_index()*2]=c;
		video[cursor_index()*2+1]=current_color;
		tty.cursor_x++;
	}
    
    /* Control del cursor */
    if(tty.cursor_x>=WIDTH)
    {
		tty.cursor_y++;
		tty.cursor_x=0;
	}
	scroll();
	if(video==VID_DIR)
		update_cursor();
}

void sys_setcolor(char c)
{
	process[current].tty.color=c;
}

void scroll()
{
	tty_t tty=process[current].tty;
	if(tty.cursor_y==24){
		/* Scroll down*/
		char *video = tty.video;
		int i;
		for(i=0;i<23*160;i++){
			video[i]=video[i+160];
		}
		while(i<24*160){
			video[i++]=' ';
			video[i++]=WHITE_TXT;
		}
		tty.cursor_y--;
	}	
}


int cursor_index()
{
	tty_t tty=process[current].tty;
	return (tty.cursor_y * WIDTH + tty.cursor_x);
}

void update_cursor(void)
{
    int index;
	index= cursor_index();
    _IO_out(0x3D4, 14);
    _IO_out(0x3D5, index >> 8);
    _IO_out(0x3D4, 15);
    _IO_out(0x3D5, index);
}

void swapTTY(int num)
{
	char auxv[160*25];
	memcpy(auxv, VID_DIR, 160*25);
	memcpy(VID_DIR, tty[num].video, 160*25);
	memcpy(tty[num.video], auxv, 160*25);
	tty[currentTTY].video=tty[num].video;
	tty[num].video=VID_DIR;
	currentTTY=num;
	update_cursor();
}

