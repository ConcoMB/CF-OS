#include "msg.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SOCKET_MSGSIZE 30

int sndMsg(int fd, void* data, int size){
	return send(fd, data, size, 0);
}

int rcvMsg(int fd, void* data, int size){
	return recv(fd, data, size, 0);
}

void create(char* id){
}

int connectChannel(char* id, int flag){
	int fd;
	struct sockaddr_in addr = {AF_INET, INADDR_ANY, INADDR_ANY};
	fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	listen(fd, 10);
	return fd;
}

int rcvString(int fd, char* data){
	int aux;
	return mq_receive(fd, data, SOCKET_MSGSIZE, &aux);
}

int sndString(int fd, char* string){
	return sndMsg(fd, (void*)string, strlen(string)+1);
}

void disconnect(int fd){
	close(fd);
}


