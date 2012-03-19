#include "msg.h"

void sndMsg(char* id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%s",id);
	fd=open(fifo, O_WRONLY);
	write(fd, data, size);
	close(fd);
}

void rcvMsg(char* id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%s",id);
	fd=open(fifo, O_RDONLY);
	read(fd, data, size);
	close(fd);
}

void connect(char* id)
{
	char fifo[10];
	sprintf(fifo, "../fifo%s",id);
	mkfifo(fifo, 0666);
}

void rcvString(char* id, char* data)
{
	int i=0;
	char c;
	char fifo[10];
	int fd;
	sprintf(fifo, "../fifo%s",id);
	fd=open(fifo, O_RDONLY);
	read(fd, &c, sizeof(char));	
	while(c)
	{
		data[i++]=c;
		read(fd, &c, sizeof(char));
	}
	data[i]='\0';
}

void sndString(char* id, char* string)
{
	sndMsg(id, (void*)string, strlen(string)+1);
}

