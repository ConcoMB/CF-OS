#ifndef __SVUTIL__
#define __SVUTIL__


#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include "files.h"
#include <pthread.h>
#include "../msg.h"
#include "league.h"
#include <unistd.h>
#include <sys/shm.h>
#include "externvars.h"
#include "clientAttendant.h"
#include "cmp.h"
#include "newMatchesListener.h"
#include <signal.h>
#include <time.h>
#include "getter.h"
#include "../colors.h"


void defChannelListener();
void quitDraft(int msg);
void newClientAssist();
void eraseDraftSems();
void clientAlive(int msg);
void clientDisconnect(int msg);
void sighandler(int sig);
void * listenClient();
void* save();
void* print();

void* channel;


#endif
