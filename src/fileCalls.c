#include "../include/fileCalls.h"
extern fileTree_t* tree;
extern fileTable_t table;

static void timeString(char buffer[], int hour, int min){
	strcpy(buffer,"01:23");
}

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
	myTree->del=0;
	strcpy(myTree->name,nameD);
	myTree->type=DIR;
	//myTree->inode.size=0;
	myTree->cantChilds=0;
	myTree->parent=dad;
	dad->childs[dad->cantChilds++]=myTree;
	/*int sector = getSector();
	if(sector==-1){
		//error
		return -8;
	}*/
	//entry.inode.sector[0]=sector;
	writeFile(myTree,0,0);	
	return 0;
}
void _ls(char* path)
{	
	int i=0;
	char spl[MAXFILES][MAXNAME];
	split(path, '/', spl);
	fileTree_t* node = getNode(spl);
	printf("%s\n", node->name);
	for(i=0; i<node->cantChilds; i++)
	{
		char color, old;
		fileEntry_t entry=ENTRY(node->childs[i]->index);
		if(!entry.del){
			old=process[current].tty->color;
			switch(entry.type){
				case DIR:
					color=0x06;
					break;
				case LINK:
					color=0x05;
					break;
				case FILE:
					color=0x02;
					break;
			}
			sys_setcolor(color);
			printf(" %s", entry.name);
			sys_setcolor(old);
			char time[6];
			timeString(time, entry.hour, entry.min);
			printf(", time: %s\n",time);
		}
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
	if(linked==0){
		return -2;
	}
	fileTree_t *newLink=malloc(sizeof(fileTree_t));
	newLink->del=0;
	newLink->cantChilds=linked->cantChilds;
	strcpy(newLink->name, linkName);

	if(linked->type==DIR)
	{
		lnChilds(linked, newLink);
	}
	//char parentName[MAXNAME];
	//removeLast(name, parentName);
	//setParent(newLink,parentName);
	fileTree_t* dad = getNode(newPath);
	if(dad==0){
		return -2;
	}
	dad->childs[dad->cantChilds++]=newLink;
	newLink->parent=dad;
	newLink->type=LINK;
	newLink->index=linked->index;
	writeFile(newLink,0,0);
	return 0;
}

int _rm(char* path, char isStr)
{
	if(strcmp(path, "/")==0){
		return -9;
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
			if(!isStr){
				delFile(node, isStr);
				return;
			}
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
	if(nodeT==0 || nodeF==0){
		return -2;
	}
	if(nodeT->type!=DIR){
		return -6;
	}
	if(isChildOf(nodeF, nodeT)){
		return -6;
	}
	removeChild(nodeF);
	nodeT->childs[nodeT->cantChilds++]=nodeF;
	nodeF->parent=nodeT;
	strcpy(nodeF->name,newName);
//	snapCP(nodeF);

	inode_t inode;
	open(nodeF, &inode);
	void* buffer = malloc(inode.size);
	readAll(&inode, &buffer);
	writeSnap(nodeF, buffer, inode.size);
	return 0;
}

int _cp(char* from, char* to)
{
	char pathFrom[MAXFILES][MAXNAME], pathTo[MAXFILES][MAXNAME], nodeName[MAXNAME];
	split(from, '/', pathFrom);
	split(to, '/', pathTo);
	fileTree_t* nodeF= getNode(pathFrom);
	if(nodeF==0){
		return -2;
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
	newNode->del=0;
	newNode->parent=dad;
	newNode->type=nodeF->type;
	strcpy(newNode->name, nodeName);
	newNode->cantChilds=nodeF->cantChilds;
	dad->childs[dad->cantChilds++]=newNode;
	if(isChildOf(newNode, nodeF)){
		return -7;
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
	if(alreadyExists(nodeName)){
		return -5;
	}
	fileTree_t* dad=getNode(path);
	if(dad==0){
		return -2;
	}
	fileTree_t* newNode=malloc(sizeof(fileTree_t));
	newNode->del=0;
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

int _attach(char* file, char* string){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		/*char nodeName[MAXNAME];
		setLastStr(path, nodeName);
		strcpy(node->name, nodeName);
		...*/
		return -2;
	}
	if(node->type==DIR || (node->type==LINK && ENTRY(ENTRY(node->index).linkTo).type!=FILE)){
		return -11;
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
			return -13;
		}

		previous.free=1;
		freeInodes(&previous);
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
		return -2;
	}
	writeEntry(j, &previous);
	//writeEntry(node->index, &ENTRY(node->index));
	//FREE(j);
	complete(dad, j);
	return 0;
}

int _cd(char* path){
	char realPath[MAXFILES][MAXNAME];
	split(path, '/', realPath);
	fileTree_t* node=getNode(realPath);
	if(node==0 || node->del){
		return -2;
	}
	if(node->type==FILE || (node->type==LINK && ENTRY(ENTRY(node->index).linkTo).type==DIR)){
		return -14;
	}
	CWD=node;
	return 0;
}


void bigFile(char* file){
	_touch(file);
	char buffer[512];
	char c='a';
	int i=4, j, z;
	for(j=0; j<i;j++){
		for(z=0; z<511; z++){
			buffer[z]=c;
		}
		buffer[z]=0;
		c++;
		_attach(file,buffer);
	}
}
int printVersions(char* file){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}
	fileEntry_t entry = ENTRY(node->index);
	while(entry.next!=-1){
		entry=ENTRY(entry.next);
	}
	int i=0;
	while(entry.prev!=-1){
		printVersion(&entry, i++);	
		entry=ENTRY(entry.prev);
	}
	return 0;
}

void printVersion(fileEntry_t * entry, int index){
	printf("Version: %d, name: %s, parent: %s, time: %d:%d\n", index, entry->name, entry->parent==-1?"root":(ENTRY(entry->parent).name), entry->hour, entry->min);
}	
