#include "msg.h"
#include <mqueue.h>
#include <errno.h>

#define MQ_MSGSIZE 30

typedef struct
{
	int mqd;
	int id;
} mq_t;

typedef struct
{
	long fromID;
	char data[MQ_MSGSIZE];
} msg_t;

int sndMsg(void* fd, void* data, int size)
{
	int i;
	mq_t* mq=(mqd_t*) fd;
	msg_t msg;
	msg->fromID=id;
	strncpy(msg->data, (char*)data, size);
	i= msgsnd(mq->mqd, (void*)&msg, sizeof(msg_t), 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

int rcvMsg(void* fd, void* data, int size)
{
	mq_t* mq=(mqd_t*) fd;
	msg_t msg;
	int i= msgrcv(mq->mqd, &msg, MQ_MSGSIZE, mq->id);
	strncpy((char*)data, msg->data, size);
	return i;
}

void createChannel(int id)
{
	msgget(id, IPC_CREAT|0666;
}

void* connectChannel(int id)
{
	mq_t *mq=malloc(sizeof(mq_t));
	mq->mqd=msgget(id, 0);
	mq->id=id;
	return (void*)mq;
}

int rcvString(void* fd, char* data)
{
	mq_t* mq=(mqd_t*) fd;
	msg_t msg;
	int i= msgrcv(mq->mqd, &msg, MQ_MSGSIZE, mq->id);
	strncpy((char*)data, msg->data, size);
	return i;
}

int sndString(void* fd, char* string)
{
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(void* fd)
{
	mq_close(*(int*)fd);
}
