#include "msg.h"

int sndMsg(int fd, void* data, int size)
{
	return write(fd, data, size);
}

int rcvMsg(int fd, void* data, int size)
{
	return read(fd, data, size);
}

void create(char* id)
{
	char fifo[10];
	sprintf(fifo, "../fifo%s",id);
	mkfifo(fifo, 0666);
}

int connect(char* id, int flag)
{
  	char fifo[10];
	sprintf(fifo, "../fifo%s",id);
	return open(fifo, flag);
}

void rcvString(int fd, char* data)
{
	int i=0;
	char c;
	read(fd, &c, sizeof(char));	
	while(c)
	{
		data[i++]=c;
		read(fd, &c, sizeof(char));
	}
	data[i]='\0';
}

void sndString(int fd, char* string)
{
	sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(int fd)
{
	close(fd);
}
