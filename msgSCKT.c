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
	int sockS, sockC;
	struct sockaddr_un cliAddr;
	struct sockaddr_un servAddr;
}sckt_t;

sckt_t * sockets[50];

int sndMsg(void* fd, void* data, int size)
{
	int addrLen= sizeof(struct sockaddr);
	sckt_t* mySock = sockets[*(int*)fd/2];
	if(*(int*)fd%2==0)
	{ //es cliente
		return sendto(mySock->sockC, data, size, 0, (struct sockaddr*)&(mySock->servAddr), addrLen);
	}
	else
	{
		return sendto(mySock->sockS, data, size, 0, (struct sockaddr*)&(mySock->cliAddr), addrLen);
	}
}

int rcvMsg(void* fd, void* data, int size)
{
	int addrLen= sizeof(struct sockaddr);
	sckt_t* mySock = sockets[*(int*)fd/2];
	if(*(int*)fd%2==1)
	{//cliente
		return recvfrom(mySock->sockC, data, size, 0, (struct sockaddr*)&(mySock->servAddr), &addrLen);
	}
	else
	{
		return recvfrom(mySock->sockS, data, size, 0, (struct sockaddr*)&(mySock->cliAddr), &addrLen);
	}
}

void createChannel(int id)
{
	if(id%2==0)
	{
		sckt_t * sock = malloc(sizeof(sckt_t));
		sock->cliAddr.sun_family=AF_UNIX;
		sock->servAddr.sun_family=AF_UNIX;
		char name[10];
		sprintf(name, "../sckt%dc", id);
		strcpy(sock->cliAddr.sun_path, name);
		sprintf(name, "../sckt%ds", id);
		strcpy(sock->servAddr.sun_path, name);
		sock->sockS=socket(AF_UNIX, SOCK_DGRAM, 0);
		sock->sockC=socket(AF_UNIX, SOCK_DGRAM, 0);
		sockets[id/2]=sock;
	}

}

void* connectChannel(int id)
{
	void* ans = &id;
	return ans;
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


