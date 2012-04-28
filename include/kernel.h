/********************************** 
*
*  Kernel
*
***********************************/

#ifndef _kernel_
#define _kernel_
#include "defs.h"
#include "kasm.h"
#include "kb.h"
#include "int80.h"
#include "timertick.h"
#include "shell.h"
#include "paging.h"
#include "libc.h"
#include "stdio.h"
#include "extras.h"
#include "scheduler.h"
#include "video.h"
#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;
typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

char _IO_in(int port);
void _IO_out(int port, char data);

#endif
