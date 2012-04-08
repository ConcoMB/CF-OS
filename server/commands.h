#ifndef __COMMANDS__
#define __COMMANDS__
#include "league.h"
#include "../common.h"
#include "draft.h"
#include "../msg.h"
#include "trade.h"
#include "display.h"
#include "join.h"
#include "externvars.h"
#include "getter.h"
#include "strQueue.h"

#define CMD_START SEND_LEAGUE

extern void (*cmds[])(client_t*);

void cmdSendLeague(client_t* myClient);
void cmdSendTeam(client_t* myClient);
void cmdSendTrade(client_t* myClient);
void cmdLeagueShow(client_t* myClient);
void cmdTeamShow(client_t* myClient);
void cmdTradeShow(client_t* myClient);
void cmdDraft(client_t* myClient);
void cmdMakeTrade(client_t* myClient);
void cmdTradeWD(client_t* myClient);
void cmdTradeYes(client_t* myClient);
void cmdTradeNeg(client_t* myClient);
void putIntoDraft(client_t* myClient);
void cmdMakeLeague(client_t* myClient);
void cmdJoinLeague(client_t* myClient);
void cmdLogout(client_t* myClient);

#endif
