#ifndef __CONNECTION__
#define __CONNECTION__
#include <stdio.h>
#include <unistd.h>
#include "../msg.h"

void connectClient(int clientID,void** channel);

#endif
