#include "../include/kernel.h"

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

<<<<<<< HEAD
	createProcess(shell, 0,0, "SHELL",  0,1,10);
	createProcess(shell, 0,0,"YPF", 1,1,10);
	//createProcess(print3, 0,0,"tabla del 3", 2,2,0);
	//createProcess(printX, 0, 0, "X", 4, 0, 1);
	//createProcess(printO, 0, 0, "O", 0, 0, 1);
=======
	createProcess(shell, 0,0, "SHELL",  0,0,10);
	createProcess(shell, 0,0,"YPF", 1,1,10);
	//createProcess(print3, 0,0,"tabla del 3", 2,2,0);
	createProcess(printX, 0, 0, "X", 4, 2, 1);
	createProcess(printO, 0, 0, "O", 0, 3, 1);
>>>>>>> 808dccb60f81d3e5a0ea5e966c33c62ad598ae75
	//createProcess(kbTest, 0, 0, "kbtest", 1, 0,0);


/* Habilito interrupcion de timer tick*/

    _mascaraPIC1(0xFC);
    _mascaraPIC2(0xFF);
	_Sti();	
	
	/*Test*/	

    while(1);
}
