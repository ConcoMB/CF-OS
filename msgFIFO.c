#include "msg.h"

typedef struct
{
	int writeD;
	int readD;
} fifo_t

int sndMsg(void* fd, void* data, int size)
{
	fifo_t* fifo=(fifo_t*)fd;
	int i=write(fifo->writeD, data, size);
	printf("sent %d:%d\n",*(int*)fd,i);
	return i;
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
  	char fifo[10];
	fifo_t* fifo=malloc(sizeof(fifo_t));
	sprintf(fifo, "../fifo%d",id);
	fifo->readD=open(fifo, O_RONLY);
	if(id%2==0)
	{
		writeID=id+1;
	}
	else
	{
		writeID=id-1;
	}
	sprintf(fifo, "../fifo%d",writeID);
	fifo->writeD=open(fifo, O_WRONLY);
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
		if(!read(*(int*)fd, &c, sizeof(char)))
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
}
