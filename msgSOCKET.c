#include "msg.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

#include <netdb.h>
#include <errno.h>
#include <string.h>

typedef struct 
{
	int socket;
	struct sockaddr_un to; 
}sckt_t;

int sndMsg(void* fd, void* data, int size)
{
	int aux = send(*(int*)fd, data, size, 0);
	return aux;
}

int rcvMsg(void* fd, void* data, int size)
{
	return recv(*(int*)fd, data, size, 0);
}

void* createChannel(int id, int flag, int mode)
{
	if(mode==SERVER)
	{
		int* fd=malloc(sizeof(int));
		char sckName[10];
		sprintf(sckName, "../sckt%s", id);
		struct sockaddr_un addr;
		addr.sun_family=AF_UNIX;
		strcpy(addr.sun_path, sckName);
		*fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	}

}

void* connectChannel()
{


}

int rcvString(void* fd, char* data)
{
	int i=0;
	char c;
	read(*(int*)fd, &c, sizeof(char));	
	while(c)
	{
		data[i++]=c;
		if(!recv(*(int*)fd, (void*)&c, sizeof(char), 0))
		{
			return i;
		}
	}
	data[i]='\0';
	return i;
}

int sndString(void* fd, char* string)
{
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(void* fd)
{
	close(*(int*)fd);
}


