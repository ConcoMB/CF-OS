CC=gcc
CFLAGS=-g
OBJ= servermain.o cmp.o joining.o ../msgFIFO.o list.o clientAttendant.o
make:
	$(CC) -o serv servermain.c cmp.c joining.c list.c ../msgFIFO.c $(CFLAGS)
	$(CC) -o att clientAttendant.c joining.c list.c cmp.c ../msgFIFO.c $(CFLAGS)
	