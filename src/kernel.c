#include "../include/kernel.h"
#include "test.c"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

void kmain() 
{
    setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x0E], 0x08, (dword)&_int_0E_hand, ACS_INT, 0);
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
	
	//createProcess(testDriveC,0,0,"TEST3", 0,0,10);
	driverPid = createProcess(FSServer,0,0,"FSServer", 1,1,10);
	//fileSyst(0,0);
	createProcess(shell, 0,0, "SHELL",  2,0,10);

	//createProcess(shell, 0,0,"YPF", 1,1,10);
	//createProcess(test, 0,0, "disktest", 0,0,10);

	//createProcess(heapTest, 0,0, "heap", 3,3,10);	
//	createProcess(printX, 0, 0, "X", 4, 2, 1);
//	createProcess(printO, 0, 0, "O", 0, 2, 1);
	//createProcess(kbTest, 0, 0, "kbtest", 1, 0,0);


/* Habilito interrupcion de timer tick*/

    _mascaraPIC1(0xFC);
    _mascaraPIC2(0xFF);
	_Sti();	
	//createProcess(fileSyst, 0, 0, "fileSyst", 0, 0, 10);

	/*Test*/	

    while(1);
}
