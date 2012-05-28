
int stackTest(int argc, char** argv){
	printf("Voy %d\n", argc);
	sleep(100);
	stackTest(argc+1,0);
	return 1;
}

int heapTest(int argc, char** argcv){
	int i=1;
	void* mem, *aux;
	while(i<20){
		if(i%3==0){
			mem=malloc(100);
			printf("malloc de 100 dio %d\n", (int)mem);
		}else if(i%3==1){
			mem= malloc(250);
			printf("malloc de 250 dio %d\n", (int)mem);
		}else if(i%3==2){
			mem= malloc(600);
			printf("malloc de 600 dio %d\n", (int)mem);
		}
		if(i%5==0){
			printf("libere %d\n", (int)aux);
			free(aux);
		}
		aux=mem;
		i++;
		sleep(500);
	}
	printHeap();
	return 0;
}

int test(int a, char** v){

	char buffer2[1024], buffer[1024];
	buffer[512]=0;
	int i;
	for(i=0;i<511;i++){
		buffer[i]=i%10+'0';
	}
	buffer[511]='x';
	_disk_write(0, buffer, 1, 1);
	_disk_read(0, buffer2, 2, 1);
	printf("primero\n");
	for(i=0 ; i<512 ;i++){
			printf("%c", buffer[i]);

	}
	printf("\nsegundo\n");
	for(i=0 ; i<1024 ;i++){
		printf("%c", buffer2[i]);

	}
	return 0;
}