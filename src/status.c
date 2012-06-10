#include "../include/status.h"


void sys_sleep(int ms)
{
	process[current].status=BLOCK;
	process[current].ticks=ms%TICK_FREQUENCY==0?ms/TICK_FREQUENCY:ms/TICK_FREQUENCY+1;
	_sys_yield();
}

void tick()
{
	int i;
	for(i=0;i<MAXPROC;i++)
	{
		if(process[i].status==BLOCK && !process[i].input && !process[i].msg)
		{
			if(process[i].ticks>0)
			{
				process[i].ticks--;
			}
			else
			{
				process[i].status=READY;
			}
		}
	}
	printTime();
}
void awake()
{
	int i;
	for(i=0; i<MAXPROC; i++)
	{
		if(process[i].status==BLOCK && process[i].input && process[i].tty==&terminals[currentTTY])
		{
			process[i].status=READY;
			process[i].input=0;
		}
	}
}

void blockInput()
{
	process[current].input=1;
	process[current].status=BLOCK;
	_sys_yield();
	//_Sti();
	//while(1);
}

void msgWAwake(){
	int i;
	for(i=0; i<MAXPROC; i++)
	{
		if(process[i].status==BLOCK && process[i].msg)	{
			process[i].status=READY;
			process[i].msg=0;
		}
	}
}

void msgRAwake(){
	process[driverPid].msg = 0;
	process[driverPid].status = READY;
}


void msgRBlock(){
	process[driverPid].msg = 1;
	process[driverPid].status = BLOCK;
	_sys_yield();
}

void msgWBlock(){
	process[current].msg = 1;
	process[current].status = BLOCK;
	_sys_yield();
}