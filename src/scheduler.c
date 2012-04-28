#include "../include/scheduler.h"

int firstTime=1;
task_t process[MAXPROC]; 
int level[MAXPROC];
//char stack[MAXPROC][STACK_SIZE];
task_t idleP;
int current, cant;

int count=0;

task_t* getProcess(int current)
{
	if(current>=0 && current<cant)
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
	if(task->pid!=-1)
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
	tick();
	process[current].status=READY;
	//printf("a");
	task_t* temp;
	//selecciona la tarea
	temp=getNextTask();
	//printf("task: %d, current %d\n", temp->pid, current);
	//temp->lastCalled=0;
	current=temp->pid;
	//last100[counter100]=CurrentPID;
	//counter100=(counter100+1)%100;
	temp->timeBlocks++;
	temp->status=RUN;
	return temp;
	
}
task_t* getCurrentProcess()
{
	return &process[current];
}

task_t* getNextTaskNFF (void)
{
	int k=current+1;
	int cantChecked=0;
	//printf("%d ",current);
	//printf("%d ", current);
	while(cantChecked<MAXPROC+1)
	{
		if(k==MAXPROC)
		{
			k=0;
		}
		//printf("%d %d",k, current);
		if(process[k].status==READY)
		{
			//printf("Proceso %d ready %d\n", k,current);
			return &process[k];
		}
		cantChecked++;
	}
	return &idleP;	
}

void swap(int a, int b)
{
	task_t aux=process[a];
	process[a]=process[b];
	process[b]=aux;
}

task_t* getNextTask()
{
	int i;
	if(cant==0)
	{
		return &idleP;
	}
	_Cli();
	while(1)
	{
		for(i=0; i<MAXPROC; i++)
		{
			if(process[i].status==READY)
			{
				if((level[i]+=process[i].priority+1)>=100)
				{
					level[i]=0;
					current=1;
					_Sti();
					return &process[i];
				}
			}
		}
	}
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
		level[i]=0;
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
	idleP.tty=&terminals[7];	
}

void cleaner(void)
{
	//printf("clean");
	/*process[current].status=FREE;
	freePage((void*)process[current].ss);
	//swap(cant, current);
	cant--;
	//YIELD
	while(1);*/
	//kill(process[current]);
}

int idle(int argc, char* argv[])
{
	
	while(1)
	{
		//printf("idle %d", count++);
		//YIELD
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

void createProcess(int (*funct)(int, char **), char* name, int p, int ttyN)
{
	int i=getFreeTask();
	task_t* task=&process[i];
	task->tty=&terminals[ttyN];
	task->pid=i;
	cant++;
	task->status=READY;
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
	task->sp=initStackFrame(funct, 0, 0, task->ss+STACK_SIZE-1, cleaner);
	task->sp->ESP=(int)(task->sp);
	task->priority=p;
	task->timeBlocks=0;
	task->name=name;
}

void createChild(int (*funct)(int, char **), int p)
{
	create
}

int sys_kill(int pid)
{
	int i=0;
	for(i=0 ; i < MAXPROC ; i++)
	{
		if(process[i].status!=FREE)
			printf("pid: %d\n", process[i].pid);
	}
	if( process[pid].status==FREE )
	{
		return 2;
	}
	freeProcessPages(pid);
	cant--;
	process[pid].status = FREE;
	return 0;
}

void sys_top(topInfo_t * topInfo)
{
	//int * processInfo = malloc(sizeof(int)+sizeof(int)*2*cant);
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
		if(process[i].status==BLOCK)
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


