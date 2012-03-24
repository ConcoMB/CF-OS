CC=gcc
CFLAGS=-g -lpthread
OBJ= servermain.o display.o cmp.o join.o ../msgFIFO.o list.o files.o clientAttendant.o arrays.o getter.o draft.o
make:
	$(CC) servermain.c -o serv cmp.c join.c getter.c files.c display.c arrays.c list.c ../msgFIFO.c $(CFLAGS)
	