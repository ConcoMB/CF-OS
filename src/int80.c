#include "../include/int80.h"


int cursor_x=0;
int cursor_y=0;

/*INT 80 RUTINE*/

void int_80(REG registers) {
	//while(1){};
	char *c;
	int i;
	switch(registers.ebx){
		case 1:
			i=registers.edx;
			c=(char*)registers.ecx;
			if(i==1){
				sys_print(*c);
			}else if(i==4){
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
		case 7:
			i=registers.ecx;
			registers.eax=(int)sys_calloc(i);
			break;
		case 8:
			i=registers.ecx;
			registers.eax=sys_free((void*)i);
			break;
		case 9:
			registers.eax=sys_heap_count();
			break;
		case 10:
			i=registers.ecx;
			sys_set_scancode(i);
			break;
		case 11:
			registers.eax=_sys_stack_count();
	}  
}

/* SCREEN FUNCTIONS */

char current_color=0x07;

int cursor_index(){
	return (cursor_y * WIDTH + cursor_x);
}

void update_cursor(void)
{
    int index;
	index= cursor_index();
    _IO_out(0x3D4, 14);
    _IO_out(0x3D5, index >> 8);
    _IO_out(0x3D4, 15);
    _IO_out(0x3D5, index);
}

void scroll(){
	if(cursor_y==24){
		/* Scroll down*/
		char *video = (char *) 0xb8000;
		int i;
		for(i=0;i<23*160;i++){
			video[i]=video[i+160];
		}
		while(i<24*160){
			video[i++]=' ';
			video[i++]=WHITE_TXT;
		}
		cursor_y--;
	}	
}


void sys_print(char c){
    char *video = (char *) 0xb8000;
    if(c=='\n'){
		cursor_y++;
		cursor_x=0;
	}
	else if(c=='\b'){
		if(cursor_x!=0){
			cursor_x--;
			video[cursor_index()*2]=' ';
		 }
	}
	else if(c=='\r'){
		cursor_x=0;
	}
    else {
		video[cursor_index()*2]=c;
		video[cursor_index()*2+1]=current_color;
		cursor_x++;
	}
    
    /* Control del cursor */
    if(cursor_x>=WIDTH){
		cursor_y++;
		cursor_x=0;
	}
	scroll();
	update_cursor();
}

void sys_setcolor(char c){
	current_color=c;
}

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*24*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=BLACK_TXT;
		i++;
	};
}

/* BUFFER FUNCTIONS */

void sys_read(char *c){
	*c=buffer_getchar();
}

/* TIME FUNCTIONS */

void sys_hour(char* hp){
	_IO_out(0x70, 0x04);
	char hour=_IO_in(0x71);
	/*BCD to binary*/
	hour = ((hour / 16) * 10) + (hour & 0xf);
	*hp=hour;
}

void sys_min(char* mp){
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

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}
