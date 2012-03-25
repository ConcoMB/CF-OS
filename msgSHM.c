#include "msg.h"
#define SIZE 100

int sndMsg(int fd, void* data, int size)
{
	
}

int rcvMsg(int fd, void* data, int size)
{
	
}

int connectChannel(char* id, int flag)
{
	char shmName[10];
	sprintf(shmName,"/shm%s",id);
	int fd=shm_open(shmName, flag|O_CREAT, 0666));
	return (int)mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
}

int rcvString(int fd, char* data)
{
	
}

int sndString(int fd, char* string)
{
	
}

void disconnect(int fd)
{
	munmap((void*)fd,SIZE);
}

void
initmutex(void)
{
	if ( !(sd = sem_open("/mutex", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}
