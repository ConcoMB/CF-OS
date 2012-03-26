#include "msg.h"
#include <mqueue.h>
#include <errno.h>

#define MQ_MSGSIZE 30

int sndMsg(void* fd, void* data, int size)
{
	int i;
	i= mq_send(*(int*)fd, (char*)data, size, 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

int rcvMsg(void* fd, void* data, int size)
{
	int i= mq_receive(*(int*)fd, (char*)data, MQ_MSGSIZE,0 );
	return i;
}

void* connectChannel(char* id, int flag)
{
	char mq[10];
	sprintf(mq, "/mq%s",id);
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MQ_MSGSIZE;
	attr.mq_flags=0;
	int *i=malloc(sizeof(int));
	*i=mq_open(mq, O_CREAT|O_RDWR, 0666, &attr);
	//printf("connected %s? %d %d\n",mq,*i,errno);
	return (void*)i;
}

int rcvString(void* fd, char* data)
{
	return mq_receive(*(int*)fd, data, MQ_MSGSIZE, 0);
}

int sndString(void* fd, char* string)
{
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(void* fd)
{
	mq_close(*(int*)fd);
}
