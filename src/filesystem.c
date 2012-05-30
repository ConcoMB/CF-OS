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
	strcpy(myTree->name,name);
	myTree->type=DIR;
	myTree->inode.size=0;
	myTree->cantChilds=0;
	setParent(myTree, parent);
	int sector = getSector();
	if(sector==-1){
		//error
		return -1;
	}
	entry.inode.sector[0]=sector;
	//ESCRIBIR EN EL DISCO
	return 0;
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

/*fileEntry_t getEntryByName(char* name)
{
	int i=0;
	for(i=0; i<MAXFILES; i++){
		if(strcmp(table.files[i].name, name)==0){
			return table.files[i];
		}
	}
	return -1;
}*/

void _ls(char* path, char ans[][MAXNAME])
{
	int i=0, k=0;
	char spl[MAXFILES][MAXNAME];
	split(path, '/', spl);
	fileTree_t* node = getNode(spl);
	for(i=0; i<node->cantChilds; i++)
	{
		strcpy(ans[i], node->childs[i]->name);
	}
	if(i!=MAXFILES){
		ans[i][0]='\0';
	}
}



void _ln(char* file, char* name)
{
	char path[MAFILES][MAXNAME], newPath[MAXFILES][MAXNAMES];
	split(file, '/', path);
	split(name, '/', newPath);
	fileTree_t *linked = getNode(path);
	fileTree_t *newLink=malloc(sizeof(fileTree_t));
	setLastStr(newPath, newLink->name);
	newLink->cantChilds=linked->cantChilds;
	if(linked->type==DIR)
	{
		cpyChilds(newLink, linked);
	}
	char c[MAXPATH];
	removeLast(name, c);
	setParent(newLink,c);
	newLink->type=LINK;
	newLink->inode=linked->inode;
}

int _rm(char* path)
{
	char realPath[MAXFILES][MAXNAME];
	split(path, '/', realPath);
	fileTree_t* node = getNode(realPath);
	_myrm(node);
}



void _myrm(fileTree_t* node){
	switch (node->type){
		case  DIR:
			rmRecursive(node);
		case FILE:
		case LINK:
			//borrar en disco
			removeChild(node);
			free(node);
			break;
	}
}

void rmRecursive(fileTree_t* node)
{
	int i=0;
	for(; i<node->cantChilds; i++){
		_myrm(node->childs[i]);
	}
}


int _mv(char* to, char* from)
{
	char pathFrom[MAXFILES][MAXNAME], pathTo[MAXFILES][MAXNAME];
	split(from, '/', pathFrom);
	split(to, '/', pathTo);
	fileTree_t* nodeF= getNode(pathFrom);
	fileTree_t* nodeT = getNode(pathTo);
	if(nodeT==0 || nodeF==0 || nodeT->type!=DIR){
		return -1;
	}
	if(isChildOf(nodeF, nodeT)){
		return -2;
	}
	nodeT->childs[nodeT->cantChilds++]=nodeF;
	removeChild(nodeF);
	nodeF->parent=nodeT; 
	return 0;
}

int _cp(char* to, char* from)
{
	char pathFrom[MAXFILES][MAXNAME], pathTo[MAXFILES][MAXNAME], nodeName[MAXNAME];
	split(from, '/', pathFrom);
	split(to, '/', pathTo);
	fileTree_t* nodeF= getNode(pathFrom);
	if(nodeF==0){
		return -1;
	}
	setLastStr(pathTo, nodeName);
	fileTree_t* dad=getNode(pathTo);
	if(dad==0){
		return -2;
	}
	fileTree_t* newNode=malloc(sizeof(fileTree_t));
	strcpy(newNode->name, nodeName);
	newNode->inode=nodeF->inode;
	newNode->type=nodeF->type;
	cpyChilds(nodeF, newNode);
	newNode->cantChilds=nodeF->cantChilds;
	newNode->parent=dad;
	dad->childs[dad->cantChilds++]=newNode;
	//SNAPSHOTS???
	return 0;
}

