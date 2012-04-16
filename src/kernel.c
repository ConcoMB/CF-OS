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
	//page_init();
	kb_init();
	srand(getmin());
	_Cli();
	initScheduler();
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
