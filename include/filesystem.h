#ifndef __FS__
#define __FS__

#include "structs.h"
#include "fileTree.h"
#include "string.h"

int getSector()

int _mkdir(char* name, char* parent);
fileEntry_t getFreeEntry();
void _ls(char* path, char ans[][MAXNAME]);
void _ln(char* file, char* name);
int _rm(char* path);
void _myrm(fileTree_t* node);
void rmRecursive(fileTree_t* node);
int _mv(char* to, char* from);
int _cp(char* to, char* from);

#endif