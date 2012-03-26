CC=gcc
CFLAGS=-g -lpthread
OBJ= servermain.o display.o cmp.o join.o ../msg$(IPC).o list.o files.o clientAttendant.o arrays.o getter.o draft.o newMatchesListener.o trade.o
make:
	$(CC) servermain.c -o serv cmp.c join.c getter.c files.c display.c arrays.c list.c newMatchesListener.c trade.c ../msg$(IPC).c $(CFLAGS)
	
