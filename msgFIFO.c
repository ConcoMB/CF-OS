void sendMsg(int id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "fifo%d",id);
	fd=open(fifo, O_WRONLY);
	write(fd, data, size);
	close(fd);
}

void rcvMsg(int id, void* data, int size)
{
	char fifo[10];
	int fd;
	sprintf(fifo, "fifo%d",id);
	fd=open(fifo, O_WRONLY);
	read(fd, data, size);
	close(fd);
}

void connect(int id)
{
	char fifo[10];
	sprintf(fifo, "fifo%d",id);
	mkfifo(fifo, 0666);
}