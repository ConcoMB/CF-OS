#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/int80.h"
#include "../include/libc.h"


/* TIMER TICK */

int ticks=19*30;

void int_08() 
{
	//printf("entre a timer\n");
	if(ticks==19*30){
		char *video = (char *) 0xb8000;
		int clock_index=(24*80+74)*2;
		
		char hour,min;
		sys_hour(&hour);
		sys_min(&min);
		
		video[clock_index+=2]=(hour/10+'0');
		video[clock_index+=2]=(hour%10+'0');
		video[clock_index+=2]=(':');
		video[clock_index+=2]=(min/10+'0');
		video[clock_index+=2]=(min%10+'0');
		ticks=0;
	}
    ticks++;
    //printf("volvi\n");
}

void timer_wait(int ms){
	int t=ms/55;
	ticks=0;
	while(ticks<t){
		//nada
	}
	return;
}
