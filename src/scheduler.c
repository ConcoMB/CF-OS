#include "../include/scheduler.h"
int firstTime=1;
task_t process[MAXPROC];
char idleStack[STACK_SIZE];
task_t idleP;
int current, cant;

int count=0;

task_t* getProcess(int current)
{
	if(current>=0 && current<cant)
		return &process[current];
	printf("else\n");
	return &idleP;
}

void printIdleStack()
{
	//printf("\nstack  %d ", count++);
	printf("\nip %d , CD %d, flags %d", (int)idleP.sp->EIP, (int)idleP.sp->CS,(int)idleP.sp->EFLAGS);
}

void saveStack(stackframe_t* sp)
{
	printf("1");
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
	task_t* temp;
	//selecciona la tarea
	temp=getNextTask();
	//temp->lastCalled=0;
	current=temp->pid;
	//last100[counter100]=CurrentPID;
	//counter100=(counter100+1)%100;
	return temp;
	
}

task_t* getNextTask (void)
{
	if(cant==0)
	{
		return &idleP;
	}
	return &idleP;
}

//Funcion que devuelve el ESP del proceso actual.
stackframe_t* getStack(task_t* proc)
{
	//printf("2");
	return proc->sp;
}

void initScheduler()
{
	cant=0;
	current=-1;
	idleP.pid=-1;
	idleP.status=READY;
	idleP.ss=(int)idleStack;
	//idleP.ssize=STACK_SIZE;
	idleP.sp=initStackFrame(idle, 0, 0, ((int)(idleStack))+STACK_SIZE-1, cleaner);
	printf("inicie\n");
}

void cleaner(void)
{
	printf("clean");
	while(1);
}

int idle(int argc, char* argv[])
{
	
	while(1)
	{
		
		printf("idle %d", count++);
		//YIELD
	}
}

stackframe_t * initStackFrame(int (*funct)(int, char **), int argc, char** argv, int bot, void(*clean)())
{
	stackframe_t * sf=(stackframe_t*)(bot-sizeof(stackframe_t));
	sf->EBP=0;
	sf->EIP=(int)funct;
	sf->CS=0x08;
	
	printf("IP:%d\n", sf->EIP);
	sf->EFLAGS=0;
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
