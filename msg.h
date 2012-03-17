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
#define USER_NAME_OCCUPIED 3
#define NAME_OR_PASSWORD_TOO_LARGE 4
#define DEFAULTID 0
#define NEWCLIENT 5

void sndMsg(int id, void* data, int size);
void rcvMsg(int id, void* data, int size);
void connect(int id);
void rcvString(int id, char* data);
void sndString(int id, char* string);
#endif