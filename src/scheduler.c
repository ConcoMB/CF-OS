#include "../include/scheduler.h"
int firstTime=1;
task_t process[MAXPROC];
char idleStack[STACK_SIZE];
task_t idleP;
int current, cant;

void saveStack(int sp)
{
	//printf("1");
	task_t* temp;
	if (!firstTime)
	{
		temp=&process[current];
		temp->sp=sp;
	}
	firstTime=0;
	return;
}

//Funcion que obtiene el ESP de idle para switchear entre tareas.
void* getIdleStack(void)
{
	return (void*)idleP.sp;
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
int getStack(task_t* proc)
{
	//printf("2");
	return proc->sp;
}

void initScheduler()
{
	cant=0;
	idleP.pid=0;
	idleP.status=READY;
	idleP.ss=(int)idleStack;
	idleP.ssize=STACK_SIZE;
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
		printf("a");
	}
}

int initStackFrame(int (*funct)(int, char **), int argc, char** argv, int bot, void(*clean)())
{
	stackframe_t * sf=(stackframe_t*)(bot-sizeof(stackframe_t));
	sf->EBP=0;
	sf->EIP=(int)funct;
	sf->CS=0x08;
	
	printf("%d,", sf->EIP);
	sf->EFLAGS=0;
	sf->retaddr=clean;
	sf->argc=argc;
	sf->argv=argv;
	return (int) sf;
}
