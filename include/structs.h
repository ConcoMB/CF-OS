#ifndef __STRUCTS__
#define __STRUCTS__
#define MAXPROC 50

#define MAXFILES 50
#define MAXSIZE 4096
#define MAXPATH 100
#define MAXNAME 20

#define MAXSECTOR 10
#define MAXSNAPSHOTS 15

#define SET(n) (bitMap[n/8]|=0x01<<(n%8))
#define GET(n) ((bitMap[n/8]>>(n%8))&0x01)
#define FREE(n) (bitMap[n/8]&= ~(0x01<<(n%8)))
#define ENTRY(n) table.files[n]
#define CWD cwd[process[current].tty->num]

typedef enum {DIR, FILE, LINK} fileType_t;
int sectorIndex;

char bitMap[MAXFILES * MAXSIZE / (512*8)];
typedef enum {RUN, BLOCK, READY, FREE} status_t;

typedef struct{
	int sector[MAXSECTOR];
	int size;
}inode_t;


typedef struct fileEntry_t{
	char name[MAXNAME];
	int parent;
	int inode;
	int prev,next;
	fileType_t type;
	char free, del;		
}fileEntry_t;


typedef struct{
	fileEntry_t files[MAXFILES];
}fileTable_t;

typedef struct fileTree_t{
	struct fileTree_t* childs[MAXFILES];
	int cantChilds;
	struct fileTree_t* parent;
	char name[MAXNAME];
	int index;
	fileType_t type;
}fileTree_t;

typedef struct
{
	int num;
	char * video;
	char color;
	int cursor_x;
	int cursor_y;
} tty_t;

typedef struct
{
	int EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX,  EIP, CS, EFLAGS;
	void* retaddr;
	int argc;
	char** argv;
} stackframe_t;

typedef struct
{
	char * name;
	status_t status;
	int ticks;
	int timeBlocks;
	int input;
	int msg;
	int pid;
	int parentid;
	int priority;
	int ss, ssize, heap, hsize;
	stackframe_t* sp;
	tty_t *tty;
} task_t;

typedef struct{
	char * name;
	int pid;
	int percent;
	int mem;
	status_t stat;
}procTopInfo_t;

typedef struct 
{
	procTopInfo_t infos[MAXPROC];
	int cant;
}topInfo_t;

fileTree_t* tree, *cwd[8];
fileTable_t table;

#endif
