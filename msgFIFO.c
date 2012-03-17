#include "msg.h"

void sndMsg(int id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%d",id);
	fd=open(fifo, O_WRONLY);
	write(fd, data, size);
	close(fd);
}

void rcvMsg(int id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%d",id);
	fd=open(fifo, O_RDONLY);
	read(fd, data, size);
	close(fd);
}

void connect(int id)
{
	char fifo[10];
	sprintf(fifo, "../fifo%d",id);
	mkfifo(fifo, 0666);
}

void rcvString(int id, char* data)
{
	int i=0;
	char c;
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%d",id);
	fd=open(fifo, O_RDONLY);
	read(fd, &c, sizeof(char));	
	while(c)
	{
		data[i++]=c;
		read(fd, &c, sizeof(char));
	}
	data[i]='\0';
}

void sndString(int id, char* string)
{
	sndMsg(id, (void*)string, strlen(string)+1);
}

