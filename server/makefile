CC=gcc
CFLAGS=-g -lpthread -Wall -lrt
OBJ= servermain.o display.o cmp.o join.o ../msg$(IPC).o severUtils.o list.o files.o commands.o strQueue.o clientAttendant.o arrays.o getter.o draft.o newMatchesListener.o trade.o
make:
	$(CC) server.c -o server cmp.c serverUtils.c join.c draft.c clientAttendant.c getter.c files.c display.c arrays.c strQueue.c list.c commands.c newMatchesListener.c trade.c ../msg$(IPC).c $(CFLAGS)
	
