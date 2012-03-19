CC=gcc
CFLAGS=-g
OBJ= servermain.o cmp.o joining.o ../msgFIFO.o list.o files.o clientAttendant.o getbyid.o
make:
	$(CC) -o serv servermain.c cmp.c joining.c getbyid.c files.c list.c ../msgFIFO.c $(CFLAGS)
	