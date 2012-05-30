
#ifndef __FTR

#define __FTR

#include "filesystem.h"
#include "structs.h"
#include "string.h"
#include "stdio.h"
#include "libc.h"

printTree(fileTree_t* aTree);
void loadTree():
void fill(fileTree_t* tree, fileTable_t* table, int myEntry);
void removeChild(fileTree_t* node);
void removeLast(char* path, char ans[MAXPATH]);
void cpyChilds(fileTree_t* from, fileTree_t* to);
fileTree_t* getNode(char path[][MAXNAME]);
int isChildOf(fileTree_t* dad, fileTree_t* son);
void setParentW(fileTree_t* newTree, char nodes[MAXFILES][MAXNAME], int index, fileTree_t* thisTree);
void setParent(fileTree_t* newTree, char* parent);

#endif