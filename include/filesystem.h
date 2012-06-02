#ifndef __FS__
#define __FS__

#include "structs.h"
#include "fileTree.h"
#include "string.h"

int getSector();
int _mkdir(char* name, char* parent);
fileEntry_t getFreeEntry(int* index);
void _ls(char* path, char ans[][MAXNAME]);
void _ln(char* file, char* name);
int _rm(char* path, char isStr);
void _myrm(fileTree_t* node, char isStr);
void rmRecursive(fileTree_t* node, char isStr);
int _mv(char* to, char* from);
int _cp(char* to, char* from);
int _touch(char* file);
int _cat(char* file);
int _revert(char* file);
void readTable();
void open(fileTree_t* node, void** buffer, int* size);
void create(fileEntry_t* entry, void* buffer, int size);
void writeSnap(fileTree_t* node, void* buffer, int size);
void writeFile(fileTree_t* node, void* buffer, int size);
void snapCP(fileTree_t* node);
void delFile(fileTree_t* node, char isStr);
int getFile(fileTree_t* node);
fileTree_t* getParentFromTable(fileEntry_t* entry);

#endif