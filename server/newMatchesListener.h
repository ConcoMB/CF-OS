#ifndef __MATCHES__
#define __MATCHES__

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include "files.h"
#include "externvars.h"


void * newMatchesListener();

void updateSportistPoints(int id, int score);

#endif
