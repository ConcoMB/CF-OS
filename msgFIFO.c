#include "msg.h"

typedef struct
{
	int writeD;
	int readD;
} fifo_t;

int sndMsg(void* fd, void* data, int size)
{
	fifo_t* fifo=(fifo_t*)fd;
	return write(fifo->writeD, data, size);
}

int rcvMsg(void* fd, void* data, int size)
{
	fifo_t* fifo=(fifo_t*)fd;
	return read(fifo->readD, data, size);
}

void createChannel(int id)
{
	char fifo[10];
	int writeID;
	sprintf(fifo, "../fifo%d",id);
	mkfifo(fifo, 0666);
	if(id%2==0)
	{
		writeID=id+1;
	}
	else
	{
		writeID=id-1;
	}
	sprintf(fifo, "../fifo%d",writeID);
	mkfifo(fifo, 0666);
}

void* connectChannel(int id)
{
	int writeID;
  	char fifoName[10];
	fifo_t* fifo=malloc(sizeof(fifo_t));
	sprintf(fifoName, "../fifo%d",id);
	fifo->readD=open(fifoName, O_RDWR);
	if(id%2==0)
	{
		writeID=id+1;
	}
	else
	{
		writeID=id-1;
	}
	sprintf(fifoName, "../fifo%d",writeID);
	fifo->writeD=open(fifoName, O_RDWR);
	return (void*)fifo;
}

int rcvString(void* fd, char* data)
{
	fifo_t* fifo=(fifo_t*)fd;
	int i=0;
	char c;
	read(fifo->readD, &c, sizeof(char));	
	while(c)
	{
		data[i++]=c;
		if(!read(fifo->readD, &c, sizeof(char)))
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
	fifo_t* fifo=(fifo_t*)fd;
	close(fifo->writeD);
	close(fifo->readD);
	free(fd);
}

void destroyChannel(int id)
{
	char fifoName[10];
	sprintf(fifoName, "../fifo%d",id);
	unlink(fifoName);
	sprintf(fifoName, "../fifo%d",id+1);
	unlink(fifoName);
}
