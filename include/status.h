#ifndef __STATS__

#define __STATS__

#include "scheduler.h"
#include "structs.h"
#include "paging.h"
#include "video.h"


void sys_sleep(int ms);
void tick();
void awake();
void blockInput();

void msgWAwake();

void msgRAwake();

void msgRBlock();

void msgWBlock();



#endif