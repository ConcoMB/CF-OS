#include "connection.h"

void connectClient(int clientID,void** writeFD, void** readFD)
{
	char readChannel[4], writeChannel[4];
	sprintf(readChannel, "%c%d", 's', clientID);
	sprintf(writeChannel, "%c%d", 'c', clientID);
	*readFD=connectChannel(readChannel, O_RDONLY);
	*writeFD=connectChannel(writeChannel, O_WRONLY);
}
