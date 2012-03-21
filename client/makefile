CC=gcc
CFLAGS=-g -lpthread
OBJ= clientmain.o listteams.o listleagues.o ../msgFIFO.o
make:
	$(CC) clientmain.c -o client  ../msgFIFO.c $(CFLAGS)
	$(CC) listteams.c -o listteams  ../msgFIFO.c $(CFLAGS)
	$(CC) listleagues.c -o listleagues  ../msgFIFO.c $(CFLAGS)