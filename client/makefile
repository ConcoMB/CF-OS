CC=gcc
CFLAGS=-g -lpthread -Wall
OBJ= clientmain.o listteams.o listleagues.o connection.o shell.o ../msg$(IPC).o
make:
	$(CC) client.c -o client.e connection.c shell.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listteams.c -o listteams.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listleagues.c -o listleagues.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) listtrades.c -o listtrades.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) ltShow.c -o ltShow.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeShow.c -o tradeShow.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) draft.c -o draft.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeNegociate.c -o tradeNegociate.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) tradeAW.c -o tradeAW.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) joinLeague.c -o joinLeague.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) makeTrade.c -o makeTrade.e connection.c ../msg$(IPC).c $(CFLAGS)
	$(CC) makeLeague.c -o makeLeague.e connection.c ../msg$(IPC).c $(CFLAGS)



