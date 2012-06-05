#include "../include/fileCalls.h"
extern fileTree_t* tree, *actual;
extern fileTable_t table;

int _mkdir(char* name, char* parent)
{
	fileTree_t* myTree = malloc(sizeof(fileTree_t));
	strcpy(myTree->name,name);
	myTree->type=DIR;
	//myTree->inode.size=0;
	myTree->cantChilds=0;
	setParent(myTree, parent);
	int sector = getSector();
	if(sector==-1){
		//error
		return -1;
	}
	//entry.inode.sector[0]=sector;
	writeFile(myTree,0,0);	
	return 0;
}
void _ls(char* path, char ans[][MAXNAME])
{
	int i=0;
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
	char path[MAXFILES][MAXNAME], newPath[MAXFILES][MAXNAME];
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
	newLink->index=linked->index;
	writeFile(newLink,0,0);
}

int _rm(char* path, char isStr)
{
	if(strcmp(path, "/")==0){
		return -1;
	}
	char realPath[MAXFILES][MAXNAME];
	split(path, '/', realPath);
	fileTree_t* node = getNode(realPath);
	if(isChildOf(node, actual)){
		return -2;
	}
	_myrm(node, isStr);
	return 0;
}



void _myrm(fileTree_t* node, char isStr){
	switch (node->type){
		case  DIR:
			rmRecursive(node, isStr);
		case FILE:
		case LINK:
			delFile(node, isStr);
			removeChild(node);
			freeNode(node);
			break;
	}
}

void rmRecursive(fileTree_t* node, char isStr)
{
	int i=0;
	for(; i<node->cantChilds; i++){
		_myrm(node->childs[i], isStr);
	}
}


int _mv(char* to, char* from)
{
	char pathFrom[MAXFILES][MAXNAME], pathTo[MAXFILES][MAXNAME], newName[MAXNAME];
	split(from, '/', pathFrom);
	split(to, '/', pathTo);
	setLastStr(pathTo, newName);
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
	strcpy(nodeF->name,newName);
	snapCP(nodeF);
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
	//newNode->inode=nodeF->inode;
	newNode->type=nodeF->type;
	cpyChilds(nodeF, newNode);
	newNode->cantChilds=nodeF->cantChilds;
	newNode->parent=dad;
	dad->childs[dad->cantChilds++]=newNode;

	inode_t inode;
	open(nodeF, &inode);
	void* buffer=malloc(inode.size);
	readAll(&inode, &buffer);
	writeSnap(newNode, buffer, inode.size);
	free(buffer);
	return 0;
}

int _touch(char* file){
	char path[MAXFILES][MAXNAME],nodeName[MAXNAME];
	split(file, '/', path);
	setLastStr(path, nodeName);
	fileTree_t* dad=getNode(path);
	if(dad==0){
		return -2;
	}
	fileTree_t* newNode=malloc(sizeof(fileTree_t));
	strcpy(newNode->name, nodeName);
	newNode->type=FILE;
	newNode->parent=dad;
	dad->childs[dad->cantChilds++]=newNode;
	writeFile(newNode,0,0);
	return 0;
}

int _cat(char* file){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}
	//inode_t in;
	//ata_read(ATA0, (void*)&in, 512, table[node->index].inode, 0);
	void * buffer;
	inode_t inode;
	open(node, &inode);
	int i=0;
	while(inode.sector[i]!=-1){
		read(&inode, i++, &buffer);
		printf("%s\n", (char*)buffer);
	}
	return 0;
}

int attatch(char* file, char* string){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		/*char nodeName[MAXNAME];
		setLastStr(path, nodeName);
		strcpy(node->name, nodeName);
		...*/
		return -1;
	}
	void* buffer;
	int len=strlen(string);
	inode_t inode;
	open(node, &inode);
	int size=inode.size+len;
	buffer=malloc(size);
	readAll(&inode, &buffer);
	memcpy(buffer+inode.size, string, len);
	writeSnap(node, buffer, size);
	free(buffer);
	return 0;
}



int revertLast(char* file){
	return revertTo(file, 1);
}

int revertTo(char* file, int version){
	if(version ==0){
		return 0;
	}
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}

	fileEntry_t this = ENTRY(node->index);
	fileEntry_t previous=this;
	int i = 0, j= node->index;
	while(i!=version){
		if(previous.prev==-1){
			return -1;
		}
		previous.free=1;
		writeEntry(j);
		FREE(j);
		j=previous.prev;
		previous = ENTRY(previous.prev);
		i++;
	}
	
	this.free=1;
	previous.next=-1;
	removeChild(node);
	freeNode(node);
	fileTree_t * dad = getParentFromTable(&previous);
	if(dad==0){
		return -3;
	}
	complete(dad, this.prev);
	writeEntry(j);
	writeEntry(node->index);
	FREE(j);
	return 0;
}