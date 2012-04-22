#include "../include/kasm.h"
#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kb.h"
#include "../include/int80.h"
#include "../include/timertick.h"
#include "../include/shell.h"
#include "../include/paging.h"
#include "../include/libc.h"
#include "../include/extras.h"
#include "../include/scheduler.h"

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
	str[1]=malloc(5);
	strcpy(str[1], "como");
	str[2]=malloc(7);
	strcpy(str[2], "estas");
	//free(str[2]);
	free(str[1]);
	str[3]=malloc(9);
	strcpy(str[3],"todobien");
	printHeap(start);
	return 0;
}

void kmain() 
{

/* Borra la pantalla. */ 

	k_clear_screen();

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
	initScheduler();
	createProcess(test);
/* Habilito interrupcion de timer tick*/

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);
        
	_Sti();	
	
	/*Test*/
	
	//shell();
	printf("initSCH\n");
        while(1)
        {
			
        }
}
