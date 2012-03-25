#include "msg.h"

int sndMsg(void* fd, void* data, int size)
{
	int i=write(*(int*)fd, data, size);
	printf("sent %d:%d\n",*(int*)fd,i);
	return i;
}

int rcvMsg(void* fd, void* data, int size)
{
	return read(*(int*)fd, data, size);
}

void* connectChannel(char* id, int flag)
{
  	char fifo[10];
	sprintf(fifo, "../fifo%s",id);
	mkfifo(fifo, 0666);
	int* fd=malloc(sizeof(int));
	*fd=open(fifo, flag);
	return (void*)fd;
}

int rcvString(void* fd, char* data)
{
	int i=0;
	char c;
	read(*(int*)fd, &c, sizeof(char));	
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
	close(*(int*)fd);
}
