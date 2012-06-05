#ifndef __FCALSS__
#define __FCALSS__

#include "structs.h"
#include "string.h"
#include "fileTree.h"

int _mkdir(char* name, char* parent);
void _ls(char* path, char ans[][MAXNAME]);
void _ln(char* file, char* name);
int _rm(char* path, char isStr);
void _myrm(fileTree_t* node, char isStr);
void rmRecursive(fileTree_t* node, char isStr);
int _mv(char* to, char* from);
int attatch(char* file, char* string);
int _cp(char* to, char* from);
int _touch(char* file);
int _cat(char* file);
int revertLast(char* file);
int revertTo(char* file, int version);

#endif