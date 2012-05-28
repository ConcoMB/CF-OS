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

int _mkdir(char* name, char* parent)
{
	fileTree_t* myTree = malloc(sizeof(fileTree_t));
	myTree.name=name;
	myTree.type=DIR;
	myTree.inode.size=0;
	myTree.cantChilds=0;
	setParent(myTree, parent);
	int sector = getSector();
	if(sector==-1){
		//error
		return -1;
	}
	entry.inode.sector[0]=sector;
	return 0;
}

void setParent(fileTree_t* newTree, char* parent){
	char nodes[MAXNAME][MAXFILES]= split(parent, "/");
	setParentW(newTree, nodes, 0, tree);
}

void setParentW(fileTree_t* newTree, char** nodes, int index, fileTree_t* thisTree)
{
	if(nodes[index]=='\0'){
		thisTree->childs[tree->cantChilds++]=newTree;
		return;
	}
	int i;
	for(i=0; i<thisTree->cantChilds; i++){
		if(strcmp(nodes[index], thisTree->childs[i]->name)==0){
			setParentW(newTree, nodes, index+1, thisTree->childs[i]);
			return;
		}
	}
}

void ln(char* file, char* name)
{
	fileEntry_t entry=getEntryByName(file);
	if(entry==-1){
		return;
	}
	fileEntry_t newEntry=getFreeEntry();
	if(newEntry==-1){
		//no hay mas
		return;
	}
	newEntry.free=1;
	newEntry.name=name;
	newEntry.type=LINK;
	newEntry.inode=entry.inode;
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
	return -1;
}

char** ls(char* path)
{
	char[MAXPATH][MAXFILES] ans;
	int i=0, k=0;
	char thisPath[MAXPATH];
	for(i=0; i<MAXFILES; i++){
		thisPath=splitPath(table[i].path);
		if(strcmp(thisPath,path)==0){
			ans[k++]=thisPath;
		}
	}
	return ans;
}

char* splitPath(char* path)
{
	int len = strlen(path);
	char ans[MAXPATH];
	for(len--;len>=0; len--){
		if(path[len]=='/'){
			int i=0;
			for(;i<len; i++){
				ans[i]=path[i];
			}
			ans[i]=0;
			return ans;
		}
	}
	return 0;
}

int rm(char* path)
{
	fileEntry_t entry = getEntryByName(path);
	if(entry==-1){
		return -1;
	}
	_rm(entry);
}

void _rm(fileEntry_t entry){
	switch (entry.type){
		case  DIR:
			rmRecursive(path);
		case FILE:
		case LINK:
			entry.free=1;
			entry.snapshots[0]=0;
			break;
	}
}

void rmRecursive(char* path)
{
	int i=0;
	for(i=0; i<MAXFILES; i++){
		if(!table[i].free && substr(path, table[i].entry.path)){
			_rm(table[i].entry);
		}
	}
}


int mv(char* to, char* from)
{

}
