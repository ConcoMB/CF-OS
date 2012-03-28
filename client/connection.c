#include "connection.h"

void connectClient(int clientID,void** channel)
{
	*channel=connectChannel(clientID+1);
}
