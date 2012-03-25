#include "msg.h"
#include <mqueue.h>

#define MQ_MSGSIZE 30

int sndMsg(int fd, void* data, int size)
{
	return mq_send(fd, (char*)data, size, 0);
}

int rcvMsg(int fd, void* data, int size)
{
	int p;
	int i= mq_receive(fd, (char*)data, MQ_MSGSIZE, &p);
	printf("msgLenght=%d",i);
	return i;
}

void create(char* id)
{
	return;
}

int connect(char* id, int flag)
{
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MQ_MSGSIZE;
	return mq_open(id, O_CREAT|flag, 0666, &attr);
}

int rcvString(int fd, char* data)
{
	int p;
	return mq_receive(fd, data, MQ_MSGSIZE, &p);
}

int sndString(int fd, char* string)
{
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(int fd)
{
	mq_close(fd);
}
