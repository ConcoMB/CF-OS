#include "../include/fileCalls.h"
extern fileTree_t* tree;
extern fileTable_t table;

int _mkdir(char* name)
{
	char spl[MAXFILES][MAXNAME];
	split(name, '/', spl);
	char nameD[MAXNAME];
	setLastStr(spl, nameD);
	fileTree_t* dad = getNode(spl);
	if(alreadyExists(nameD)){
		return -5;
	}
	fileTree_t* myTree = malloc(sizeof(fileTree_t));
	strcpy(myTree->name,nameD);
	myTree->type=DIR;
	//myTree->inode.size=0;
	myTree->cantChilds=0;
	myTree->parent=dad;
	dad->childs[dad->cantChilds++]=myTree;
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



int _ln(char* file, char* name)
{
	char path[MAXFILES][MAXNAME], newPath[MAXFILES][MAXNAME], linkName[MAXNAME];
	split(file, '/', path);
	split(name, '/', newPath);
	setLastStr(newPath, linkName);
	if(alreadyExists(linkName)){
		return -5;
	}
	fileTree_t *linked = getNode(path);
	fileTree_t *newLink=malloc(sizeof(fileTree_t));
	newLink->cantChilds=linked->cantChilds;
	strcpy(linkName, newLink->name);
	if(linked->type==DIR)
	{
		lnChilds(linked, newLink);
	}
	char parentName[MAXNAME];
	removeLast(name, parentName);
	setParent(newLink,parentName);
	newLink->type=LINK;
	newLink->index=linked->index;
	writeFile(newLink,0,0);
	return 0;
}

int _rm(char* path, char isStr)
{
	if(strcmp(path, "/")==0){
		return -1;
	}
	char realPath[MAXFILES][MAXNAME];
	split(path, '/', realPath);
	fileTree_t* node = getNode(realPath);
	if(isChildOf(node, CWD)){
		return -3;
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
	if(alreadyExists(newName)){
		return -5;
	}
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

int _cp(char* from, char* to)
{
	char pathFrom[MAXFILES][MAXNAME], pathTo[MAXFILES][MAXNAME], nodeName[MAXNAME];
	split(from, '/', pathFrom);
	split(to, '/', pathTo);
	fileTree_t* nodeF= getNode(pathFrom);
	if(nodeF==0){
		return -1;
	}
	setLastStr(pathTo, nodeName);
	if(alreadyExists(nodeName)){
		return -5;
	}
	fileTree_t* dad=getNode(pathTo);
	if(dad==0){
		return -2;
	}
	
	fileTree_t* newNode=malloc(sizeof(fileTree_t));
	//newNode->inode=nodeF->inode;
	newNode->parent=dad;
	newNode->type=nodeF->type;
	strcpy(newNode->name, nodeName);
	newNode->cantChilds=nodeF->cantChilds;
	dad->childs[dad->cantChilds++]=newNode;
	if(isChildOf(newNode, nodeF)){
		return -3;
	}
	cpyChilds(nodeF, newNode);
	if(nodeF->type!=DIR){
		inode_t inode;
		open(nodeF, &inode);
		void* buffer=malloc(inode.size);
		readAll(&inode, &buffer);
		writeFile(newNode, buffer, inode.size);
		free(buffer);
	}else{
		writeFile(newNode, 0,0);
	}
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
	printf("CAT\n");
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}
	//inode_t in;
	//ata_read(ATA0, (void*)&in, 512, table[node->index].inode, 0);
	void * buffer=malloc(512);
	inode_t inode;
	open(node, &inode);
	int i=0;
	while(inode.sector[i]!=-1){
		//printf("sector %d: %d, \n", i, inode.sector[i]);
		read(&inode, i++, &buffer);
		printf("%s\n", (char*)buffer);
	}
	free(buffer);
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
	if(node->type!=FILE){
		return -2;
	}
	void* buffer;
	int len=strlen(string);
	inode_t inode;
	open(node, &inode);
	inode.size+=len;
	buffer=malloc(inode.size);
	readAll(&inode, &buffer);
	memcpy(buffer+(inode.size-len), string, len);
	writeSnap(node, buffer, inode.size);
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
	fileEntry_t previous = ENTRY(node->index);
	int i = 0, j= node->index;
	while(i!=version){
		if(previous.prev==-1){
			return -1;
		}
		previous.free=1;
		FREE(j);
		writeEntry(j, &previous);
		j=previous.prev;
		previous = ENTRY(j);
		i++;
	}

	previous.next=-1;
	removeChild(node);
	freeNode(node);
	fileTree_t * dad = getParentFromTable(&previous);
	if(dad==0){
		return -3;
	}
	writeEntry(j, &previous);
	//writeEntry(node->index, &ENTRY(node->index));
	//FREE(j);
	complete(dad, j);
	return 0;
}
