#ifndef __CATT__
#define __CATT__

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "join.h"
#include "league.h"
#include <sys/shm.h>
#include <signal.h>
#include "commands.h"
#include "externvars.h"
#include "../colors.h"

void* clientAtt(void* arg);

void makeConnection(client_t* myClient);
void start(client_t* myClient);
void logClient(client_t* myClient);
void makeDisconnection(client_t* myClient);
int controlDraft(draft_t* draft);
void* keepAlive(void* arg);
void draftManager(client_t* myClient);


#endif
