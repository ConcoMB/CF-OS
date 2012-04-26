/*********************************************
kasm.h

************************************************/

#ifndef KASM__
#define KASM__
#include "defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

void		_mascaraPIC1 (byte mascara);  /* Escribe mascara de PIC1 */
void		_mascaraPIC2 (byte mascara);  /* Escribe mascara de PIC2 */

void		_Cli(void);        /* Deshabilita interrupciones  */
void		_Sti(void);	 /* Habilita interrupciones  */

void		_int_08_hand();      /* Timer tick */
void		_int_80_hand();      /* System calls */
void 		_int_09_hand();
void		_int_0E_hand();
void		_lcr3(int* adress);
void		_epag();
void		_fill_page1();
int		_sys_stack_count();
void _sys_yield();

void		_debug (void);

#endif
