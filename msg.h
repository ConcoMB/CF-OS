#ifndef __MSG__
#define __MSG__

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define INCORRECT_PASSWORD 1
#define USER_NOT_FOUND 2
#define LOGIN 1
#define SIGNUP 2
#define NAME_OCCUPIED 3
#define NAME_OR_PASSWORD_TOO_LARGE 4
#define DEFAULTID 0
#define NEWCLIENT 5
#define USER_ALREADY_JOINED 12

void sndMsg(char* id, void* data, int size);
void rcvMsg(char* id, void* data, int size);
void connect(char* id);
void rcvString(char* id, char* data);
void sndString(char* id, char* string);
#endif