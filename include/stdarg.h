/* DEFINICIONES PARA ARGUMENTOS VARIABLES */

#ifndef STDARG_H
#define STDARG_H


#ifndef RC_INVOKED

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
#endif

#ifndef	_VA_LIST
#define _VA_LIST
typedef char* va_list;
#endif

#define __va_argsiz(t)	\
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#ifdef	__GNUC__

#define va_start(ap, pN)	\
	((ap) = ((va_list) __builtin_next_arg(pN)))
#else

#define va_start(ap, pN)	\
	((ap) = ((va_list) (&pN) + __va_argsiz(pN)))
#endif

#define va_end(ap)	((void)0)


#define va_arg(ap, t)					\
	 (((ap) = (ap) + __va_argsiz(t)),		\
	  *((t*) (void*) ((ap) - __va_argsiz(t))))

#endif 
#endif 

#ifndef IO
#define IO
#define stdin 0
#define stdout 1

#endif
