#include "msg.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <errno.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

typedef struct 
{
	int scktDesc, id;
	struct sockaddr_un dest;
}sckt_t;

void* aux;
int count=0;

int sndMsg(void* fd, void* data, int size)
{
	sckt_t* sock= (sckt_t*)fd;
	sendto(sock->scktDesc, data, size, 0, (struct sockaddr *)&sock->dest, sizeof(struct sockaddr_un));
}

int rcvMsg(void* fd, void* data, int size)
{
	sckt_t* sock= (sckt_t*)fd;
	int len = sizeof(struct sockaddr_un);
	recvfrom(sock->scktDesc, data, size, 0, (struct sockaddr *)&sock->dest, &len);
}

void createChannel(int id)
{

}


void* connectChannel(int id)
{
	int sendID;
	if(id%2==0) //server
	{
		sendID=id+1;
	}
	else
	{
		sendID=id-1;
	}
	sckt_t * sckt = malloc(sizeof(sckt_t));
	sckt->dest.sun_family=AF_UNIX;
	char name[10];
	sprintf(name, "../sckt%d", sendID);
	strcpy(sckt->dest.sun_path, name);
	if(!(sckt->scktDesc=socket(AF_UNIX, SOCK_DGRAM, 0)))
	{
		printf("Cannot create socket\n");
		exit(1);
	}
	printf("socket %d\n", id);
	struct sockaddr_un myAddr;
	myAddr.sun_family=AF_UNIX;
	sprintf(name, "../sckt%d", id);
	strcpy(myAddr.sun_path, name);
	if((bind(sckt->scktDesc, (struct sockaddr *)&myAddr, sizeof(struct sockaddr_un))))
	{
		printf("Cannot bind socket %d\n", errno);
		exit(1);
	}
	sckt->id=id;
	return (void*)sckt;
}

int rcvString(void* fd, char* data)
{
	sckt_t* sock= (sckt_t*)fd;
	int i=0;
	char c;
	int len = sizeof(struct sockaddr_un);
	recvfrom(sock->scktDesc, &c, sizeof(char), 0, (struct sockaddr *)&sock->dest, &len);

	while(c)
	{
		data[i++]=c;
		if(!(recvfrom(sock->scktDesc, &c, sizeof(char), 0, (struct sockaddr *)&sock->dest, &len)))
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
	char name[10];
	sprintf(name, "../sckt%d", ((sckt_t*)fd)->id);
	close(((sckt_t*)fd)->scktDesc);
	unlink(name);
}


