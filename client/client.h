#ifndef __CLIH__
#define __CLIH__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "../msg.h"
#include "../common.h"
#include "connection.h"
#include "shell.h"
#include "../colors.h"

void sighandler(int sig);
void userLog();
void start();
int signClient(char * name);
int logClient(char * name);
void makeDefConnection();
void* channel;
void* defChannel;
int msgID;
void* keepAlive(void* arg);

#endif
