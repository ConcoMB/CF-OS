#include "connection.h"

void connectClient(int clientID,void** writeFD, void** readFD)
{
	*readFD=connectChannel(clientID+1);
	*writeFD=connectChannel(clientID);
}
