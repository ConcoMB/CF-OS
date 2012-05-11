#include "../include/scheduler.h"

#define __PRIOR__

int firstTime=1;
task_t process[MAXPROC]; 
int level[MAXPROC];
//char stack[MAXPROC][STACK_SIZE];
task_t idleP;
int current, cant;

int count=0;

#ifndef __PRIOR__

task_t* getNextTask(void)
{
	int k=current+1;
	int cantChecked=0;
	if(cant==0)
	{
		return &idleP;
	}
	while(cantChecked<MAXPROC+1)
	{
		if(k==MAXPROC)
		{
			k=0;
		}
		if(process[k].status==READY)
		{
			return &process[k];
		}
		//printf("-%d",k);
		cantChecked++;
		k++;
	}
	return &idleP;	
}

#else

task_t* getNextTask()
{
	int i, nobody=1;
	if(cant==0)
	{
		return &idleP;
	}
	//_Cli();
	while(1)
	{
		nobody=1;
		for(i=0; i<MAXPROC; i++)
		{
			if(process[i].status==READY)
			{
				nobody=0;
				if((level[i]+=(5-process[i].priority))>=100)
				{
					level[i]=0;
					current=1;
					//_Sti();
					return &process[i];
				}
			}
		}
		if(nobody)
		{
			return &idleP;
		}
	}
}

#endif

task_t* getProcess(int current)
{
	if(current>=0 && current<MAXPROC)
		return &process[current];
	return &idleP;
}

void printIdleStack()
{
	//printf("\nstack  %d ", count++);
	printf("\nip %d , CD %d, flags %d", (int)idleP.sp->EIP, (int)idleP.sp->CS,(int)idleP.sp->EFLAGS);
}

void saveStack(stackframe_t* sp)
{
	task_t* temp, * aux=getProcess(current);
	if (!firstTime)
	{
		temp=getProcess(current);
		temp->sp=sp;
	}
	firstTime=0;
	stackResize(aux);
	return;
}

void stackResize(task_t* task)
{
	if(task->pid!=-1 && task->status!=FREE)
	{
		//printf("ESP = %d\n", task->sp->ESP);
		int max= task->ssize*4096;
		int now = (max-(task->sp->ESP) % max);
		double percent = (double)(now) / (double)(max);
		//printf("num %d / den %d\n", aux, task->ssize*4096);
		if(percent>=0.8)
		{
			//printf("ojo  y \n");
			getStackPage(task->pid);
			task->ssize++;
		}
	}
}

//Funcion que obtiene el ESP de idle para switchear entre tareas.
void* getIdleStack(void)
{
	return idleP.sp;
}

//Funcion que devuelve el PROCESS* siguiente a ejecutar
task_t* getNextProcess (void)
{
	if(process[current].status==RUN)
	{
		process[current].status=READY;
	}
	task_t* temp;
	temp=getNextTask();
	//temp->lastCalled=0;
	current=temp->pid;
	//last100[counter100]=CurrentPID;
	//counter100=(counter100+1)%100;
	temp->timeBlocks++;
	temp->status=RUN;
	tick();
	return temp;
	
}
task_t* getCurrentProcess()
{
	return &process[current];
}


//Funcion que devuelve el ESP del proceso actual.
stackframe_t* getStack(task_t* proc)
{
	return proc->sp;
}

void initScheduler()
{
	int i;
	for(i=0;i<MAXPROC;i++)
	{
		//level[i]=0;
		process[i].status=FREE;
	}
	cant=0;
	current=-1;
	idleP.pid=-1;
	idleP.status=READY;
	idleP.ss=(int)getStackPage(idleP.pid);
	//initHeap((void*)idleP.heap);
	//idleP.ssize=STACK_SIZE;
	idleP.sp=initStackFrame(idle, 0, 0, idleP.ss+STACK_SIZE-1, cleaner);
	idleP.tty=&terminals[2];	
}



int idle(int argc, char* argv[])
{
	
	while(1)
	{
		//_sys_yield();
		_Sti();
	}
}

stackframe_t * initStackFrame(int (*funct)(int, char **), int argc, char** argv, int bot, void(*clean)())
{
	stackframe_t * sf=(stackframe_t*)(bot-sizeof(stackframe_t));
	sf->EBP=0;
	sf->EIP=(int)funct;
	sf->CS=0x08;
	
	//printf("IP:%d\n", sf->EIP);
	sf->EFLAGS=512;
	sf->retaddr=clean;
	sf->argc=argc;
	sf->argv=argv;
	return sf;
}


void printStack(int a, int b, int c)
{
	printf("\n%d %d %d\n", a, b ,c);
}

void* getIP()
{
	return &idle;
}

int getFreeTask(void)
{
	int i;
	for(i=0;i<MAXPROC;i++)
	{
		if(process[i].status==FREE)
		{
			return i;
		}
	}
	return 0;
}

void createProcess(int (*funct)(int, char **), int argc, char** argv, char* name, int p, int ttyN, int parid)
{
	int i=getFreeTask();
	task_t* task=&process[i];
	level[i]=0;
	task->tty=&terminals[ttyN];
	task->pid=i;
	cant++;
	task->status=READY;
	task->parentid=parid;
	//printf("tnego pid %d\n", task->pid);

	task->ss=(int)getStackPage(task->pid);
	task->ssize=1;
	task->heap=(int)getHeapPage(task->pid);
	if(task->ss==0 || task->heap==0)
	{
		//ERROR
		return;
	}
	initHeap((void*)task->heap);
	task->sp=initStackFrame(funct, argc, argv, task->ss+STACK_SIZE-1, cleaner);
	task->sp->ESP=(int)(task->sp);
	task->priority=p;
	task->timeBlocks=0;
	task->input=0;
	task->name=name;
}

void createChild(int (*funct)(int, char **), int argc, char ** argv)
{
    task_t * task = &process[current];
    createProcess(funct,argc,argv,argv[0],task->priority,task->tty->num, task->pid);
}


void cleaner(void)
{

	_Cli();
	sys_kill(current);
	_Sti();
	_sys_yield();
}


int sys_kill(int pid)
{
	int i;
	if(process[pid].status==FREE)
	{
		return 2;
	}
	if(pid<0 || pid>=MAXPROC)
	{
		return 1;
	}
	freeProcessPages(pid);
	process[pid].status = FREE;
	cant--;
	for(i=0; i<MAXPROC; i++)
	{
		if(process[i].status!=FREE && process[i].parentid==pid)
		{
			sys_kill(i);
		}
	}
	_sys_yield();
	return 0;
}

void sys_top(topInfo_t * topInfo)
{
	task_t proc;
	int i=0, k=0,aux=0;
	for(;i < MAXPROC;i++)
	{
		proc = process[i];
		if(proc.status != FREE)
		{
			topInfo->names[k]=proc.name;
			topInfo->pids[k]=proc.pid;
			topInfo->percent[k]= proc.timeBlocks;
			aux += proc.timeBlocks;
			k++;
		}
	}
	topInfo->cant=k;
	for(i=0;i<k;i++)
	{
		topInfo->percent[i] *= 100.0 / aux;
	}
}

int processHasFocus()
{
	//printf("focus %d\n", process[current].tty==&terminals[currentTTY]);
	return process[current].tty==&terminals[currentTTY];
}

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
		if(process[i].status==BLOCK && !process[i].input)
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
