#include "msg.h"
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#define SHM_SIZE 4000
#define BUFFER_S 800



typedef struct
{
	sem_t* sem; //semaforo para leer/escribir
	sem_t* changes; //semaforo para indicar cambios
	void* mem;
} shm_t;

typedef struct
{
	shm_t* write;
	shm_t* read;
} shmDesc_t;


shm_t* cChannel(int id);

int created=0;
int mapped=0;
void* startMem=NULL;

int sndMsg(void* fd, void* data, int size)
{
	fflush(stdout);
	int i;
	shmDesc_t *shmd=(shmDesc_t*)fd;
	shm_t* shm=shmd->write;
	sem_wait(shm->sem);
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
	sem_post(shm->sem);
	sem_post(shm->changes);
	return size;
}

int rcvMsg(void* fd, void* data, int size)
{
	int i;
	shmDesc_t *shmd=(shmDesc_t*)fd;
	shm_t* shm=shmd->read;
	int *head=(int*)shm->mem;
	int *tail=(int*)(shm->mem+sizeof(int));
	sem_wait(shm->changes);
	sem_wait(shm->sem);
	for(i=0;i<size;i++)
	{
		if(*tail==*head)
		{
			break;
		}
		((char*)data)[i]=((char*)shm->mem)[*tail];
		(*tail)++;
		if(*tail>=BUFFER_S)
		{
			*tail=sizeof(int)*2;
		}
	}
	sem_post(shm->sem);
	//sleep(1);
	return i;
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
		void* mem=mmap(NULL, BUFFER_S, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		ftruncate(fd, SHM_SIZE);
		close(fd);
		memset(mem, 0, SHM_SIZE);
		for(i=0; i<SHM_SIZE; i+=BUFFER_S)
		{
			((int*)(mem+i))[0]=sizeof(int)*2;
			((int*)(mem+i))[1]=sizeof(int)*2;
			//printf("%p:%d %p:%d -",&((int*)(mem+i))[0], ((int*)(mem+i))[0], &((int*)(mem+i))[1], ((int*)(mem+i))[1]);
		}
		//munmap(mem, SIZE);
		startMem=mem;
		mapped=1;
		//printf("Created shm\n");
	}
}

void* connectChannel(int id)
{
	int writeID;
	if(id%2==0)
	{
		writeID=id+1;
	}
	else
	{
		writeID=id-1;
	}
	shmDesc_t* shmd=malloc(sizeof(shmDesc_t));
	shmd->read=cChannel(id);
	shmd->write=cChannel(writeID);
	return (void*) shmd;
}

shm_t* cChannel(int id)
{
	//printf("Trying to connect ...");
	char semName[10];
	shm_t* shm=malloc(sizeof(shm_t));
	if(!mapped)
	{
		int fd;
		fd=shm_open("/shm", O_RDWR, 0666);
		startMem=mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		close(fd);
		mapped=1;
	}
	sprintf(semName,"/mutex%d",id);
	shm->sem=sem_open(semName, O_RDWR|O_CREAT, 0666, 1);
	sprintf(semName,"/mutexCh%d",id);
	shm->changes=sem_open(semName, O_RDWR|O_CREAT, 0666, 0);
	shm->mem=startMem+id*BUFFER_S;
	if(startMem==MAP_FAILED)
	{
		printf("Mapping failure: %d\n",errno);
		exit(-1);
	}
	else
	{
		/*printf("Mapped to %p (head:%d tail:%d)\n", shm->mem, ((int*)shm->mem)[0], ((int*)shm->mem)[1]);
		int j;
		for(j=0;j<BUFFER_S; j++)
		{
			printf("%d ",(int)((char*)shm->mem)[j]);
		}
		printf("\n");*/
	}
	return shm;
}

int rcvString(void* fd, char* data)
{
	shmDesc_t *shmd=(shmDesc_t*)fd;
	shm_t* shm=shmd->read;
	int i=0;
	sem_wait(shm->changes);
	sem_wait(shm->sem);
	int *head=(int*)shm->mem;
	int *tail=(int*)(shm->mem+sizeof(int));
	while(*head!=*tail)
	{
		data[i]=((char*)shm->mem)[*tail];
		(*tail)++;
		if(data[i]==0)
		{
			sem_post(shm->sem);
			return i+1;
		}
		if(*tail>=SHM_SIZE)
		{
			*tail=sizeof(int)*2;
		}
		i++;
	}
	sem_post(shm->sem);
	return i;
}

int sndString(void* fd, char* string)
{
	return sndMsg(fd,string,strlen(string)+1);
}

void disconnect(void* fd)
{
	shm_t *shm=(shm_t*)fd;
	munmap(shm->mem,BUFFER_S);
	sem_close(shm->sem);
	sem_close(shm->changes);
}

void destroyChannel(int id)
{
	if(created)
	{
		shm_unlink("/shm");
		created=0;
	}
}
