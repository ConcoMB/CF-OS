#include "../include/kernel.h"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */


/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

void printHeap(void* start)
{
	void* dir=start;
	while(1)
	{
		int* front=dir-3*sizeof(int);
		int* back=dir-2*sizeof(int);
		int* free=dir-sizeof(int);
		printf("Free:%d  Front:%d  Back:%d\n",*free,*front,*back);
		if(*front==0)
		{
			return;
		}
		int i;
		for(i=0; i<*front; i++)
		{
			printf("%c ",*(char*)(dir+i));
		}
		printf("\n");
		dir+=*front+3*sizeof(int);
	}
}

int test(int argc, char** argv)
{
	char* str[4];
	str[0]=malloc(10);
	void* start=str[0];
	strcpy(str[0], "hola");
	str[1]=malloc(20);
	strcpy(str[1], "como");
	str[2]=malloc(7);
	strcpy(str[2], "estas");
	malloc(4000);
	malloc(4000);
	//free(str[2]);
	//str[3]=malloc(15);
	//strcpy(str[3],"todo");
	//free(str[2]);
	free(str[0]);
	free(str[1]);
	//printHeap(start);
	int i=0;
	while((i++)!=100000);
	printf("termine");
	return 0;
}
int test2(int argc, char** argv)
{
	while(1)
	{
		printf("aa");
	}
}

void kmain() 
{

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

        setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
		setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
		setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
		setup_IDT_entry (&idt[0x0E], 0x08, (dword)&_int_0E_hand, ACS_INT, 0);

/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);
	page_init();
	kb_init();
	srand(getmin());
	_Cli();
	initScreens();
	initScheduler();
	createProcess(test2, 4);
	createProcess(test, 2);
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);
        
	_Sti();	
	
	/*Test*/
	
	//shell();
	//printf("initSCH\n");
        while(1)
        {
			
        }
}
