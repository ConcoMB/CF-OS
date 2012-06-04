#include "../include/filesystem.h"
extern fileTree_t* tree, *actual;
extern fileTable_t table;

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


fileEntry_t getFreeEntry(int* index)
{
	int i=0;
	for(i=0; i<MAXFILES; i++)
	{
		if(table.files[i].free){
			*index = i;
			return table.files[i];
		}
	}
	//no hay lugar
	return table.files[i];

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

int _revert(char* file){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}
	fileEntry_t this = table.files[node->index];
	if(this.prev==-1){
		return -1;
	}
	fileEntry_t prev = table.files[this.prev];
	this.free=1;
	prev.next=-1;
	removeChild(node);
	freeNode(node);
	fileTree_t * dad = getParentFromTable(&prev);
	if(dad==0){
		return -3;
	}
	complete(dad, this.prev);
	//escribir en this.prev y en node->index
	return 0;
}

fileTree_t* getParentFromTable(fileEntry_t* entry){
	char path[MAXPATH];
	path[0]='\0';
	buildPath(path, entry);
	char splPath[MAXFILES][MAXNAME];
	split(path, '/', splPath);
	return getNode(splPath);
}

/*
void _edit(char* file)
{
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	char nodeName[MAXNAME];
	setLastStr(path, nodeName);
	fileTree_t* dirN = getNode(path);
	if(dirN==0){
		return -2;
	}
	int i;
	for(i=0; i<dirN->cantChilds; i++)
	{
		if(strcmp(dirN->childs[i]->name, nodeName)==0){
			editFile(dirN->childs[i]);
			return;
		}
	}
	fileTree_t* newNode = malloc(sizeof(fileTree_t));
	strcpy(newNode->name,nodeName);
	create(newNode);
	newNode->type=FILE;
	newNode->cantChilds=0;
	newNode->parent=dirN;
	dirN->childs[dirN->cantChilds++]=newNode;
	//SNAPSHOTS???
	editFile(newNode);
	return;
}



void editFile(fileTree_t* node){
	inode_t in;
	ata_read(ATA0, &in, 512, table[node->index].inode, 0);
	int i;
	int size;
	void * buffer;
	open(in, &buffer, &size);
	for(i=0; i<inode.size; i++){
		ata_read(ATA0, buffer+i*512, 512, inode.sector[i], 0);
	}
	clearSceen();
	resetCursor();
	printf("%s\n", (char*)buffer);
}
*/


void readTable(){
	ata_read(ATA0, table.files, sizeof(fileEntry_t)*MAXFILES, 0,0);
}

void open(fileTree_t* node, inode_t* inode){
	ata_read(ATA0, (void*)&inode, sizeof(inode), table.files[node->index].inode,0);
	/**buffer=malloc(inode.size*512);
	int i;
	for(i=0; i<inode.size; i++){
		ata_read(ATA0, (*buffer)+i*512, 512, inode.sector[i],0);
	}*/
}

void read(inode_t* inode, int which, void** buffer){
	int size=512;
	if(inode->sector[which+1]==-1){
		size=inode->size%512;
	}
	ata_read(ATA0, *buffer, size, inode->sector[which], 0);
}

void readAll(inode_t* inode, void** buffer){
	int i=0, size;
	while(inode->sector[i]!=-1){
		size=512;
		if(i!=MAXSECTOR && inode->sector[i+1]==-1){
			size=inode->size%512;
		}
		ata_read(ATA0, *buffer+i*512, size, inode->sector[i], 0);
	}
}

void write(inode_t* inode, int which, int size, void* buffer){
	ata_read(ATA0, buffer, size, inode->sector[which], 0);
}

void create(fileEntry_t* entry, void* buffer, int size, int index){
	int sects = size/512;
	size%512!=0?sects++:1;
	inode_t inode;
	inode.size=size;
	int i, j;
	for(i=0; i<sects; i++){
		j=getSector();
		inode.sector[i]=j;
		ata_write(ATA0, buffer+i*512,512, j,0);
	}
	int inodeSect=getSector();
	entry->inode=inodeSect;
	//write en la tabla el inodo y el enntry

}

void writeSnap(fileTree_t* node, void* buffer, int size){
	int i;
	fileEntry_t old= table.files[node->index];
	fileEntry_t entry=getFreeEntry(&i);
	strcpy(entry.name, node->name);
	entry.type=node->type;
	entry.parent=node->parent->index;
	entry.free=0;
	entry.del=0;
	entry.prev=node->index;
	old.next=i;
	entry.next=-1;
	create(&entry, buffer, size, i);
	node->index=i;
}

void writeFile(fileTree_t* node, void* buffer, int size){
	int i;
	fileEntry_t entry=getFreeEntry(&i);
	node->index=i;
	entry.next=-1;
	entry.prev=-1;
	entry.type=node->type;
	entry.parent=node->parent->index;
	entry.free=0;
	entry.del=0;
	strcpy(entry.name, node->name);
	if(size!=0)
		create(&entry, buffer, size, i);
}

void snapCP(fileTree_t* node){
	int i;
	fileEntry_t entry=getFreeEntry(&i);
	fileEntry_t old= table.files[node->index];
	old.next=i;
	entry.prev=node->index;
	entry.next=-1;
	entry.inode=old.inode;
	node->index=i;
	//write en la tabla en i
}

void delFile(fileTree_t* node, char isStr){
	if(!isStr){
		int i = getFile(node);
		table.files[i].del=1;
		//escribir en i
	}else{
		fileEntry_t entry = table.files[node->index];
		while(entry.prev!=-1){
			entry.free=1;
			entry=table.files[entry.prev];
		}
		entry.free=1;
		//escribir toda la tabla
	}
	
}

int getFile(fileTree_t* node)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(strcmp(node->name, table.files[i].name)==0){
			while(table.files[i].next!=-1){
				i=table.files[i].next;
			}
			return i;
		}
	}
	return -1;
}
