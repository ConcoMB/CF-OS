#include "../include/int80.h"



/*INT 80 RUTINE*/

void int_80(REG registers) 
{
	//while(1){};
	char *c;
	int i;
	switch(registers.ebx)
	{
		case 1:
			i=registers.edx;
			c=(char*)registers.ecx;
			if(i==1)
			{
				sys_print(*c);
			}
			else if(i==4)
			{
				/*sys_print(*c);*/
				sys_speak(*c);
			}
			break;
		case 2:
			c=(char*)registers.ecx;
			sys_read(c);
			break;
		case 3:
			c=(char*)registers.ecx;
			sys_hour(c);
			break;
		case 4:
			c=(char*)registers.ecx;
			sys_min(c);
			break;
		case 5:
			c=(char*)registers.ecx;
			sys_setcolor(*c);
			break;
		case 6:
			i=registers.ecx;
			registers.eax=(int)sys_malloc(i);
			break;
		/*case 7:
			i=registers.ecx;
			registers.eax=(int)sys_calloc(i);
			break;*/
		case 8:
			i=registers.ecx;
			registers.eax=sys_free((void*)i);
			break;
		/*case 9:
			registers.eax=sys_heap_count();
			break;*/
		case 10:
			i=registers.ecx;
			sys_set_scancode(i);
			break;
		case 11:
			registers.eax=_sys_stack_count();
			break;
		case 14:
			/*SLEEP*/
			i=registers.ecx;
			sys_sleep(i);
			break;
		case 12:
			i=registers.ecx;
			registers.eax=sys_kill(i);
			break;
		case 13:
			sys_top((topInfo_t*)registers.ecx);
	}  
}

/* BUFFER FUNCTIONS */

void sys_read(char *c)
{
	*c=buffer_getchar();
}

/* TIME FUNCTIONS */

void sys_hour(char* hp)
{
	_IO_out(0x70, 0x04);
	char hour=_IO_in(0x71);
	/*BCD to binary*/
	hour = ((hour / 16) * 10) + (hour & 0xf);
	*hp=hour;
}

void sys_min(char* mp)
{
	_IO_out(0x70,0x02);
	char min=_IO_in(0x71);
	/*BCD to binary*/
	min = ((min / 16) * 10) + (min & 0xf);
	*mp=min;
}

/* IDT */

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,byte cero) 
{
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

