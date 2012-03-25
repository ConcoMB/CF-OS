#include "msg.h"
#include <mqueue.h>
#include <errno.h>

#define MQ_MSGSIZE 30

int sndMsg(int fd, void* data, int size)
{
	int i;
	i= mq_send(fd, (char*)data, size, 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

int rcvMsg(int fd, void* data, int size)
{
	int i= mq_receive(fd, (char*)data, MQ_MSGSIZE,0 );
	return i;
}

void create(char* id)
{
	return;
}

int connect(char* id, int flag)
{
	char mq[10];
	sprintf(mq, "/mq%s",id);
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MQ_MSGSIZE;
	attr.mq_flags=0;
	int i= mq_open(mq, O_CREAT|O_RDWR, 0666, &attr);
	printf("connected %s? %d %d\n",mq,i,errno);
	return i;
}

int rcvString(int fd, char* data)
{
	return mq_receive(fd, data, MQ_MSGSIZE, 0);
}

int sndString(int fd, char* string)
{
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(int fd)
{
	mq_close(fd);
}
