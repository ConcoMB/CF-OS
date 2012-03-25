CC=gcc
<<<<<<< HEAD
CFLAGS=-g -lpthread -lrt
OBJ= servermain.o display.o cmp.o join.o ../msg$(IPC).o list.o files.o clientAttendant.o arrays.o getter.o draft.o
make:
	$(CC) servermain.c -o serv cmp.c join.c getter.c files.c display.c arrays.c list.c ../msg$(IPC).c $(CFLAGS)
=======
CFLAGS=-g -lpthread
OBJ= servermain.o display.o cmp.o join.o ../msgFIFO.o list.o files.o clientAttendant.o arrays.o getter.o draft.o newMatchesListener.o
make:
	$(CC) servermain.c -o serv cmp.c join.c getter.c files.c display.c arrays.c list.c newMatchesListener.c ../msgFIFO.c $(CFLAGS)
>>>>>>> 638aeedb63597396ab548c0d3c5dd14f624ca917
	
