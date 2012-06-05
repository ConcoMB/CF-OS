#ifndef __FS__
#define __FS__

#include "structs.h"
#include "fileTree.h"
#include "string.h"

int getSector();
fileEntry_t getFreeEntry(int* index);
int snapList(char* file);
void readTable();
void initTable();
void open(fileTree_t* node, inode_t* inode);
void read(inode_t* inode, int which, void** buffer);
void write(inode_t* inode, int which, int size, void* buffer);
void create(fileEntry_t* entry, void* buffer, int size, int index);
void writeSnap(fileTree_t* node, void* buffer, int size);
void writeFile(fileTree_t* node, void* buffer, int size);
void snapCP(fileTree_t* node);
void delFile(fileTree_t* node, char isStr);
int getFile(fileTree_t* node);
fileTree_t* getParentFromTable(fileEntry_t* entry);
void readAll(inode_t* inode, void** buffer);
void initialize();
void initBitMap();
void writeEntry(int index);
void writeInode(fileEntry_t* entry, inode_t* inode);
void writeBitMap();
void readBitMap();
#endif