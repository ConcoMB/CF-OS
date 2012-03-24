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

#include "msg.h"

int sndMsg(int fd, void* data, int size);
int rcvMsg(int fd, void* data, int size);
void create(char* id);
int connect(char* id, int flag);
int rcvString(int fd, char* data);
int sndString(int fd, char* string);
void disconnect(int fd);

#endif
