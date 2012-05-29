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


void setParentW(fileTree_t* newTree, char nodes[MAXFILES][MAXNAME], int index, fileTree_t* thisTree)
{
	if(nodes[index][0]=='\0'){
		thisTree->childs[thisTree->cantChilds++]=newTree;
		newTree->parent=thisTree;
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
void setParent(fileTree_t* newTree, char* parent){
	char nodes[MAXNAME][MAXFILES];
	split(parent, '/', nodes);
	setParentW(newTree, nodes, 0, tree);
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

void cpyChilds(fileTree_t* from, fileTree_t* to)
{
	int i;
	for(i=0; i<from->cantChilds; i++)
	{
		to->childs[i]=from->childs[i];
	}
}

fileTree_t* getNode(char path[][MAXNAME])
{
	int i=0;
	fileTree_t* myTree=tree;
	while(path[i][0]!='\0'){
		int j;
		for(j=0; j<myTree->cantChilds; j++){
			if(strcmp(myTree->childs[j]->name, path[i])==0){
				myTree=myTree->childs[j];
			}
		}
		i++;
	}
	return myTree;
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

void removeLast(char* path, char ans[MAXPATH])
{
	int len = strlen(path);
	for(len--;len>=0; len--){
		if(path[len]=='/'){
			int i=0;
			for(;i<=len; i++){
				ans[i]=path[i];
			}
			ans[i]=0;
			return;
		}
	}
	return;
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

void removeChild(fileTree_t* node)
{
	int i;
	fileTree_t* dad=node->parent;
	for(i=0; i<dad->cantChilds; i++){
		if(node==dad->childs[i]){
			//shifteo
			int j=i;
			for(;j<dad->cantChilds-1; j++){
				dad->childs[j]=dad->childs[j+1];
			}
			dad->cantChilds--;
			return;
		}
	}
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


int mv(char* to, char* from)
{

}
