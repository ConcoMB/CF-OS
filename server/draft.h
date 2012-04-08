#ifndef __DRAFTH__
#define __DRAFTH__


#include <pthread.h>
#include <time.h>
#include "getter.h"
#include "../common.h"
#include "league.h"
#include "display.h"
#include "../colors.h"


void * draftAttendant(void* arg1);
int setForcedSportist(league_t* league, team_t* team);
void* sportistReader(void* arg1);
int sendToClient(client_t* client, int msg);
void draftEnd(draft_t* draft);
void draftBegin(draft_t* draft);
void listenQuitters(draft_t* draft);
void updateInfo(draft_t* draft);
void clientsWait(draft_t* draft, int leagueSize);




#endif
