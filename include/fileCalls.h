#ifndef __FCALSS__
#define __FCALSS__

#include "structs.h"
#include "string.h"
#include "fileTree.h"

int _mkdir(char* name);
void _ls(char* path);
int _ln(char* file, char* name);
int _rm(char* path, char isStr);
void _myrm(fileTree_t* node, char isStr);
void rmRecursive(fileTree_t* node, char isStr);
int _mv(char* to, char* from);
int _attach(char* file, char* string);
int _cp(char* from, char* to);
int _touch(char* file);
int _cat(char* file);
int revertLast(char* file);
int revertTo(char* file, int version);
void bigFile(char* file);
int _cd(char* file);
int printVersions(char* file);
void printVersion(fileEntry_t * entry, int index);
#endif
