#include "msg.h"
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#define SIZE 1000
#define BUFFER_S 50

static sem_t* initMutex(char* id);
static void enter(sem_t* sem);
static void leave(sem_t* sem);

int created=0;
int mapped=0;
void* startMem=NULL;

typedef struct
{
	sem_t* sem;
	void* mem;
} shm_t;

int sndMsg(void* fd, void* data, int size)
{
	int i;
	shm_t *shm=(shm_t*)fd;
	printf("Enter ");
	enter(shm->sem);
	int *head=(int*)shm->mem;
	for(i=0;i<size;i++)
	{
		((char*)shm->mem)[*head]=((char*)data)[i];
		(*head)++;
		if(*head>=BUFFER_S)
		{
			*head=sizeof(int)*2;
		}
	}
	leave(shm->sem);
	printf("Leave\n");
	fflush(stdout);
	return size;
}

int rcvMsg(void* fd, void* data, int size)
{
	int i,bytes=0;
	shm_t *shm=(shm_t*)fd;
	while(bytes==0)
	{
		enter(shm->sem);
		int *head=(int*)shm->mem;
		int *tail=(int*)(shm->mem+sizeof(int));
		for(i=0;i<size;i++)
		{
			//printf("h %d t %d\n",*head, *tail);
			if(*tail==*head)
			{
				break;
			}
			((char*)data)[i]=((char*)shm->mem)[*tail];
			(*tail)++;
			bytes++;
			if(*tail>=BUFFER_S)
			{
				*tail=sizeof(int)*2;
			}
		}
		leave(shm->sem);
		sleep(2);
	}
	return bytes;
}

void createChannel(int id)
{
	if(!created)
	{
		int i;
		created=1;
		char shmName[10];
		int fd;
		sprintf(shmName,"/shm");
		fd=shm_open(shmName, O_RDWR|O_CREAT, 0666);
		void* mem=mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		ftruncate(fd, SIZE);
		for(i=0; i<SIZE; i+=BUFFER_S)
		{
			((int*)mem+i)[0]=sizeof(int)*2;
			((int*)mem+i)[1]=sizeof(int)*2;
		}
		munmap(mem, SIZE);
		printf("Created shm\n");
	}
}

void* connectChannel(int id)
{
	printf("Trying to connect ...");
	char semName[10];
	int fd;
	shm_t* shm=malloc(sizeof(shm_t));
	fd=shm_open("/shm", O_RDWR, 0666);
	if(!mapped)
	{
		startMem=mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		mapped=1;
	}
	sprintf(semName, "%d", id);
	shm->sem=initMutex(semName);
	close(fd);
	shm->mem=startMem+id*BUFFER_S;
	if(startMem==MAP_FAILED)
	{
		printf("Mapping failure: %d\n",errno);
		exit(-1);
	}
	else
	{
		printf("Mapped to %p\n", shm->mem);
	}
	return (void*)shm;
}

int rcvString(void* fd, char* data)
{
	int i=0, bytes=0;
	shm_t *shm=(shm_t*)fd;
	while(bytes==0)
	{
		enter(shm->sem);
		int *head=(int*)shm->mem;
		int *tail=(int*)(shm->mem+sizeof(int));
		while(*head!=*tail)
		{
			data[i]=((char*)shm->mem)[*tail];
			(*tail)++;
			bytes++;
			if(data[i]==0)
			{
				leave(shm->sem);
				return i+1;
			}
			if(*tail>=SIZE)
			{
				*tail=sizeof(int)*2;
			}
			i++;
		}
		leave(shm->sem);
	}
	return bytes;
}

int sndString(void* fd, char* string)
{
	return sndMsg(fd,string,strlen(string)+1);
}

void disconnect(void* fd)
{
/*	shm_t *shm=(shm_t*)fd;
	munmap(shm->mem,SIZE);
	sem_close(shm->sem);*/
}

static sem_t* initMutex(char* id)
{
	char semName[10];
	sprintf(semName,"/mutex%s",id);
	return sem_open(semName, O_RDWR|O_CREAT, 0666, 1);
}

static void enter(sem_t* sem)
{
	sem_wait(sem);
}

static void leave(sem_t* sem)
{
	sem_post(sem);
}
