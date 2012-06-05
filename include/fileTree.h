
#ifndef __FTR

#define __FTR

#include "filesystem.h"
#include "fileCalls.h"
#include "structs.h"
#include "string.h"
#include "stdio.h"
#include "libc.h"

void printTree(fileTree_t* aTree);
void loadTree();
void fill(fileTree_t* tree, int myEntry);
void complete(fileTree_t* dad, int index);
void removeChild(fileTree_t* node);
void freeNode(fileTree_t* node);
void removeLast(char* path, char ans[MAXPATH]);
void cpyChilds(fileTree_t* from, fileTree_t* to);
fileTree_t* getNode(char path[][MAXNAME]);
int isChildOf(fileTree_t* dad, fileTree_t* son);
void setParentW(fileTree_t* newTree, char nodes[MAXFILES][MAXNAME], int index, fileTree_t* thisTree);
void setParent(fileTree_t* newTree, char* parent);
void buildPath(char path[], fileEntry_t* entry);
#endif