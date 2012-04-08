CC=gcc
CFLAGS=-g -lpthread -Wall -lrt
make:
	$(CC) ./server/server.c -o ./server/server ./server/cmp.c ./server/join.c ./server/getter.c ./server/files.c ./server/draft.c ./server/clientAttendant.c ./server/serverUtils.c ./server/display.c ./server/arrays.c ./server/strQueue.c ./server/list.c ./server/commands.c ./server/newMatchesListener.c ./server/trade.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/client.c -o ./client/client ./client/connection.c ./client/shell.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/listteams.c -o ./client/listteams ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/listleagues.c -o ./client/listleagues ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/listtrades.c -o ./client/listtrades ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/ltShow.c -o ./client/ltShow ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/tradeShow.c -o ./client/tradeShow ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/draft.c -o ./client/draft ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/tradeNegociate.c -o ./client/tradeNegociate ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/tradeAW.c -o ./client/tradeAW ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/joinLeague.c -o ./client/joinLeague ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/makeTrade.c -o ./client/makeTrade ./client/connection.c msg$(IPC).c $(CFLAGS)
	$(CC) ./client/makeLeague.c -o ./client/makeLeague ./client/connection.c msg$(IPC).c $(CFLAGS)

	
