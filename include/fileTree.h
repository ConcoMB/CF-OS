
#ifndef __FTR

#define __FTR

#include "filesystem.h"

typedef struct fileTree_t{
	struct fileTree_t* childs[MAXFILES];
	int cantChilds;
	struct fileTree_t* parent;
	char name[MAXNAME];
	inode_t inode;
	fileEntry_t snapshots[MAXSNAPSHOTS];
	fileType_t type;
}fileTree_t;

#endif