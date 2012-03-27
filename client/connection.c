#include "connection.h"

void connectClient(int clientID,void** writeFD, void** readFD)
{
	*writeFD=connectChannel(clientID);
	*readFD=connectChannel(clientID+1);
}
