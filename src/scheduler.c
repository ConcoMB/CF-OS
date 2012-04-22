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
	task_t* temp;
	if (!firstTime)
	{
		temp=getProcess(current);
		temp->sp=sp;
	}
	firstTime=0;
	return;
}

//Funcion que obtiene el ESP de idle para switchear entre tareas.
void* getIdleStack(void)
{
	return idleP.sp;
}

//Funcion que devuelve el PROCESS* siguiente a ejecutar
task_t* getNextProcess (void)
{
	//printf("a");
	task_t* temp;
	//selecciona la tarea
	temp=getNextTask();
	//printf("task: %d, current %d\n", temp->pid, current);
	//temp->lastCalled=0;
	current=temp->pid;
	//last100[counter100]=CurrentPID;
	//counter100=(counter100+1)%100;
	return temp;
	
}

task_t* getNextTask (void)
{
	int k=current+1;
	int cantChecked=0;
	//printf("%d ",current);
	//printf("%d ", current);
	while(cantChecked<cant+1)
	{
		if(k==cant)
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

task_t* getNextTaskFF()
{
	int i;
	if(cant==0)
	{
		return &idleP;
	}
	_Cli();
	while(1)
	{
		for(i=0; i<cant; i++)
		{
			if(process[i].status==READY)
			{
				if((level[i]+=process[i].priority+1)>=100)
				{
					level[i]=0;
					current=1;
					_Sti();
					return process[i];
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
	idleP.ss=(int)getPage();
	//idleP.ssize=STACK_SIZE;
	idleP.sp=initStackFrame(idle, 0, 0, idleP.ss+STACK_SIZE-1, cleaner);
	
	printf("inicie\n");
}

void cleaner(void)
{
	printf("clean");
	process[current].status=FREE;
	freePage((void*)process[current].ss);
	swap(cant, current);
	cant--;
	//YIELD
	while(1);
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

void createProcess(int (*funct)(int, char **), int p)
{
	_Cli();
	int i=getFreeTask();
	task_t* task=&process[i];
	task->pid=cant++;
	task->status=READY;
	task->ss=(int)getPage();
	task->sp=initStackFrame(funct, 0, 0, task->ss+STACK_SIZE-1, cleaner);
	task->priority=p;
	_Sti();
}
