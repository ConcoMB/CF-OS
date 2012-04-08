#include "server.h"

league_t** leagues;
int lCant, uCant;
sem_t* saveSem;
user_t** users;
listADT clients;
strQueue_t printQueue;

int nextClientID=2;

int main()
{
	saveSem=sem_open("/mutexSave", O_RDWR|O_CREAT, 0666, 1);
	signal(SIGABRT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGINT, sighandler);
	clients=newList(cmpClient);
	loadAll();
	printQueue=newQueue();
	pthread_t saveThread,clThread,newMatchFilesThread, printThread;
	pthread_create(&clThread, NULL, listenClient, NULL);
	pthread_create(&printThread, NULL, print, NULL);
	pthread_create(&saveThread, NULL, save, NULL);
	pthread_create(&newMatchFilesThread, NULL, newMatchesListener, NULL);
	pthread_join(clThread, NULL);
	pthread_join(printThread, NULL);
	pthread_join(saveThread, NULL);
	pthread_join(newMatchFilesThread, NULL);
	return 0;
}
