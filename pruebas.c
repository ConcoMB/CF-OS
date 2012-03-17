#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "list.h"
#include <pthread.h>
#include "server.h"
#include "league.h"

int main()
{
    int aux, fd, otro;
    mkfifo("./pruebafifo", 0666);
    if(fork())
    {
        fd=open("./pruebafifo", O_WRONLY);
        aux=12;
        write(fd, aux, sizeof(int));
    }
    else
    {
        fd=open("./pruebafifo", O_RDONLY);
        read(fd, otro, sizeof(int));
        printf("%d\n", otro);
        exit(0);
    }
    return 0;
}