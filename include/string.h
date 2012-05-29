#ifndef __STR__
#define __STR__

#include "libc.h"

int strcmp(char* str1, char* str2);
int substr(char* str1, char* str2);
void strcpy(char* dest, char* src);
char* itoa(int val, char* buffer);
char* xtoa(int mem, char* buffer);
int atoi(char* a);
int strlen(char* s);
void split ( char *string, const char sep, char list[][MAXFILES]);
void setLastStr(char path[][MAXFILES], char dest[MAXNAME]);

#endif
