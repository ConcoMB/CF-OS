CC=gcc
CFLAGS=-g -lpthread -lrt
OBJ= clientmain.o listteams.o listleagues.o ../msgFIFO.o
make:
	$(CC) clientmain.c -o client  ../msg$(IPC).c $(CFLAGS)
	$(CC) listteams.c -o listteams  ../msg$(IPC).c $(CFLAGS)
	$(CC) listleagues.c -o listleagues  ../msg$(IPC).c $(CFLAGS)
	$(CC) listtrades.c -o listtrades  ../msg$(IPC).c $(CFLAGS)
	$(CC) ltShow.c -o ltShow  ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeShow.c -o tradeShow  ../msg$(IPC).c $(CFLAGS)
	$(CC) draft.c -o draft  ../msg$(IPC).c $(CFLAGS)


