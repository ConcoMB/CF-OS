#include "../include/filesystem.h"

fileTable_t table;
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

void initTable(){
	//table=?
	int i;
	for(i=0; i<MAXFILES; i++){
		table[i].free=0;
	}
}

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

int mkdir(char* path, char* name)
{
	fileEntry_t entry = getFreeEntry();
	if( entry==-1){
		return -2;
	}
	entry.free=0;
	entry.path=path;
	entry.name=name;
	entry.isDir=1;
	entry.inode.link=1;
	entry.inode.size=0;
	int sector = getSector();
	if(sector==-1){
		//error
		return -1;
	}
	entry.inode.sector[0]=sector;
	return 0;
}

void ln(char* path, char* name)
{
	fileEntry_t entry=getEntryByName(name);
	if(entry==0){
		return;
	}
	entry.inode.link++;
}

fileEntry_t getFreeEntry()
{
	int i=0;
	for(i=0; i<MAXFILES; i++)
	{
		if(table.files[i].free){
			return table.files[i];
		}
	}
	return -1;
}

fileEntry_t getEntryByName(char* name)
{
	int i=0;
	for(i=0; i<MAXFILES; i++){
		if(strcmp(table.files[i].name, name)==0){
			return table.files[i];
		}
	}
	return 0;
}

