#include "msg.h"
#include <mqueue.h>
#include <errno.h>
#include <sys/msg.h>

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
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	msg.fromID=mq->id;
	strncpy(msg.data, (char*)data, size);
	i= msgsnd(mq->mqd, (void*)&msg, sizeof(msg_t), 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	printf("Sent with id: %i (%d)\n",(int)msg.fromID,i);
	return i;
}

int rcvMsg(void* fd, void* data, int size)
{
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	printf("Recieving with id: %d...",mq->id);
	int i= msgrcv(mq->mqd, &msg, MQ_MSGSIZE, mq->id,0);
	printf("%d\n",i);
	strncpy((char*)data, msg.data, size);
	return i;
}

void createChannel(int id)
{
	int key;
	key=ftok("../msg.h",0);
	if(msgget(key, IPC_CREAT|0666)==-1)
	{
		printf("Error creating Message Queue with key: %d - errno: %d\n", key, errno);
		exit(-1);
	}
	printf("Created queue %d\n", key);
}

void* connectChannel(int id)
{
	mq_t *mq=malloc(sizeof(mq_t));
	int key;
	key=ftok("../msg.h",0);
	mq->mqd=msgget(key, 0666);
	mq->id=id+1;
	if(mq->mqd==-1)
	{
		printf("Error connecting to Message Queue with key %d - errno: %d\n", key, errno);
		exit(-1);
	}
	printf("Connected to queue %d\n",key);
	return (void*)mq;
}

int rcvString(void* fd, char* data)
{
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	int i=msgrcv(mq->mqd, &msg, MQ_MSGSIZE, mq->id,0);
	strcpy((char*)data, msg.data);
	return i;
}

int sndString(void* fd, char* string)
{
	int i;
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	msg.fromID=mq->id;
	strcpy(msg.data, string);
	i= msgsnd(mq->mqd, (void*)&msg, sizeof(msg_t), 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

void disconnect(void* fd)
{
	mq_t* mq=(mq_t*) fd;
	msgctl(mq->mqd, IPC_RMID, NULL);
}
