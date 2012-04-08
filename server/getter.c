#include "getter.h"

trade_t* getTradeByID(league_t* league, int searchID)
{
  trade_t* trade;
  reset(league->trades);
  while((trade=getNext(league->trades))!=NULL)
  {
    if(trade->ID==searchID)
    {
      return trade;
    }
  }
  return NULL;
}

team_t* getTeamByClient(league_t* league, client_t* client)
{
  int i;
  if(client!=NULL)
  {
    for(i=0; i<league->tCant; i++)
    {
      if(league->teams[i]->user->ID==client->user->ID)
      {
        return league->teams[i];
      }
    }
  }
  return NULL;
}

client_t* getClientByID(int id)
{
	client_t* client;
	reset(clients);
	while((client=getNext(clients))!=NULL)
	{
		if(client->ID==id)
		{
			return client;
		}
	}
	return NULL;
}
