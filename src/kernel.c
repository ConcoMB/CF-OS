#include "../include/kernel.h"
#include "test.c"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */


/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/
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
	
	//initializeFS();

	//fileSyst(0,0);
	//createProcess(fileSyst, 0, 0, fileSyst, 0, 4, 10);
	createProcess(shell, 0,0, "SHELL",  2,0,10);
	createProcess(shell, 0,0,"YPF", 1,1,10);
	//createProcess(test, 0,0, "disktest", 0,0,10);

	//createProcess(heapTest, 0,0, "heap", 3,3,10);	
//	createProcess(printX, 0, 0, "X", 4, 2, 1);
//	createProcess(printO, 0, 0, "O", 0, 2, 1);
	//createProcess(kbTest, 0, 0, "kbtest", 1, 0,0);


/* Habilito interrupcion de timer tick*/

    _mascaraPIC1(0xFC);
    _mascaraPIC2(0xFF);
	_Sti();	
	
	/*Test*/	

    while(1);
}
