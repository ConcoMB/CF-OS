#include "msg.h"
#include <mqueue.h>
#include <errno.h>
#include <sys/msg.h>

#define MSGSIZE 300

typedef struct
{
	int mqd;
	int readID;
	int writeID;
} mq_t;

typedef struct
{
	long int fromID;
	char data[MSGSIZE];
} msg_t;

int connected=0;

int sndMsg(void* fd, void* data, int size)
{
	int i;
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	msg.fromID=mq->writeID;
	strncpy(msg.data, (char*)data, size);
	i= msgsnd(mq->mqd, (void*)&msg, MSGSIZE, 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

int rcvMsg(void* fd, void* data, int size)
{
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	//printf("Recieving with id: %d...",mq->id);
	int i= msgrcv(mq->mqd, &msg, MSGSIZE, mq->readID,0);
	//printf("%d (errno: %d)\n",i, errno);
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
	//printf("Created queue %d\n", key);
}

void* connectChannel(int id)
{
	mq_t *mq=malloc(sizeof(mq_t));
	int key;
	mq->readID=id+1;
	if(id%2==0)
	{
		mq->writeID=id+2;
	}
	else
	{
		mq->writeID=id;
	}
	if(connected)
	{
		mq->mqd=connected;
	}
	else
	{
		key=ftok("../msg.h",0);
		mq->mqd=msgget(key, 0666);
		connected=mq->mqd;
	}
	if(mq->mqd==-1)
	{
		printf("Error connecting to Message Queue with key %d - errno: %d\n", key, errno);
		exit(-1);
	}
	//printf("Connected to queue %d\n",key);
	return (void*)mq;
}

int rcvString(void* fd, char* data)
{
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	int i=msgrcv(mq->mqd, &msg, MSGSIZE, mq->readID,0);
	strcpy((char*)data, msg.data);
	return i;
}

int sndString(void* fd, char* string)
{
	int i;
	mq_t* mq=(mq_t*) fd;
	msg_t msg;
	msg.fromID=mq->writeID;
	strcpy(msg.data, string);
	i= msgsnd(mq->mqd, (void*)&msg, MSGSIZE, 0);
	if(i==-1)
	{
		printf("Send Error: errno %d\n", errno);
	}
	return i;
}

void disconnect(void* fd)
{
	free(fd);
}

void destroyChannel(int id)
{
	if(connected)
	{
		msgctl(connected, IPC_RMID, NULL);
		connected=0;
	}
}
