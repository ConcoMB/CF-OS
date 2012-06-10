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
#include "filesystem.h"
#include "int80.h"
#include "timertick.h"
#include "shell.h"
#include "paging.h"
#include "libc.h"
#include "stdio.h"
#include "extras.h"
#include "scheduler.h"
#include "disc.h"
#include "video.h"
#include "msg.h"
#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;
typedef enum eINT_80 {WRITE=0, READ} tINT_80;
typedef enum eUSER {U_KERNEL=0, U_NORMAL} tUSERS;

char _IO_in(int port);
void _IO_out(int port, char data);
char _IO_in_w(int port);
void _IO_out_w(int port, char data);

void _port_out(unsigned short port, unsigned short data);
unsigned char _port_in(unsigned short port);
void _portw_out(unsigned short port, unsigned short data);
unsigned int _portw_in(unsigned short port);

#endif
