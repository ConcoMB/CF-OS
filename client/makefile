CC=gcc
CFLAGS=-g -lpthread -lrt
OBJ= clientmain.o listteams.o listleagues.o connection.o ../msgFIFO.o
make:
	$(CC) clientmain.c -o client connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listteams.c -o listteams connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listleagues.c -o listleagues connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listtrades.c -o listtrades connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) ltShow.c -o ltShow connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeShow.c -o tradeShow connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) draft.c -o draft connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeNegociate.c -o tradeNegociate connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeAW.c -o tradeAW connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) joinLeague.c -o joinLeague connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) makeTrade.c -o makeTrade connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) makeLeague.c -o makeLeague connection.c ../msg$(IPC).c $(CFLAGS)



