#include "../include/filesystem.h"
/*
void printMap(){
	printf("%x\t", bitMap[0]);
		printf("%x\t", bitMap[1]);
	printf("%x\n", bitMap[2]);

}

int main(){
	printMap();
	SET(1);
	SET(3);
	SET(4);
	printMap();
	FREE(4);
	printMap();
	printf("%d %d",GET(0),GET(1));
}
*/

int getSector()
{
	int i;
	int flag = 0;
	for(i=sectorIndex; flag && i==sectorIndex; i++){
		flag=1;
		if(i==(MAXFILES * MAXSIZE / (512*8))){
			i=0;
		}
		if(GET(i)==0){
			SET(i);
			sectorIndex=i;
			return i;
		}
	}
	//DISCO LLENO
	return -1;
}


