#ifndef __FS__
#define __FS__

#define MAXFILES 100
#define MAXSIZE 4096
#define MAXNAME 20
#define MAXPATH 100
#define MAXSECTOR 10

#define SET(n) (bitMap[n/8]|=0x01<<(n%8))
#define GET(n) ((bitMap[n/8]>>(n%8))&0x01)
#define FREE(n) (bitMap[n/8]&= ~(0x01<<(n%8)))

typedef struct{
	int sector[MAXSECTOR];
	int size;
	int link;
}inode_t;

typedef struct{
	char path[MAXPATH];
	inode_t inode;
	char name[MAXNAME];
	int version;
	int fileID;
	char isDir;		
}fileEntry_t;

typedef struct{
	fileEntry_t files[MAXFILES];
}fileTable_t;

int sectorIndex;
char bitMap[MAXFILES * MAXSIZE / (512*8)];

#endif