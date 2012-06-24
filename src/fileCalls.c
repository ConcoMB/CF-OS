#include "../include/fileCalls.h"
extern fileTree_t* tree;
extern fileTable_t table;

static int getCWDR(char* buffer, fileTree_t* node);

void getCWD(char* buffer){
	fileTree_t* node=CWD;
	int len=getCWDR(buffer, node);
	buffer[len]=0;
}

static int getCWDR(char* buffer, fileTree_t* node){
	int len=0;
	if(node->parent!=node){
		len=getCWDR(buffer, node->parent);
		strcpy(buffer+len, node->name);
		len+=strlen(node->name);
	}
	buffer[len++]='/';
	return len;
}

static void timeString(char buffer[], int hour, int min){
	//strcpy(buffer,"01:23");
	char h0, h1, m0, m1;
	h1=hour%10+'0';
	m1=min%10+'0';
	h0=hour/10+'0';
	m0=min/10+'0';
	buffer[0]=h0;
	buffer[1]=h1;
	buffer[2]=':';
	buffer[3]=m0;
	buffer[4]=m1;
	buffer[5]=0;
}
static void* mallocFS(int bytes){
	int pid=current;
	current=driverPid;
	void* ans = malloc(bytes);
	current=pid;
	return ans;
}

static void freeFS(void* object){
	int pid=current;
	current=driverPid;
	free(object);
	current=pid;
}



int _mkdir(char* name)
{
	char spl[MAXFILES][MAXNAME];
	split(name, '/', spl);
	char nameD[MAXNAME];
	setLastStr(spl, nameD);
	fileTree_t* dad = getNode(spl);
	if(alreadyExists(nameD, dad)){
		return -5;
	}
	if(dad->type==LINK){
		dad=findLink(dad);
	}
	if(dad->type==FILE){
		return -2;
	}
	fileTree_t* myTree = mallocFS(sizeof(fileTree_t));
	myTree->del=0;
	strcpy(myTree->name,nameD);
	myTree->type=DIR;
	myTree->cantChilds=0;
	myTree->parent=dad;
	dad->childs[dad->cantChilds++]=myTree;
	
	writeFile(myTree,0,0);	
	return 0;
}
void _ls(char* path)
{	
	int i=0;
	char spl[MAXFILES][MAXNAME];
	split(path, '/', spl);
	fileTree_t* node = getNode(spl);
	for(i=0; i<node->cantChilds; i++)
	{
		char color, old;
		fileEntry_t entry=ENTRY(node->childs[i]->index);
		if(!entry.del){
			old=process[current].tty->color;
			switch(entry.type){
				case DIR:
					color=0x0A;
					break;
				case LINK:
					color=0x05;
					break;
				case FILE:
					color=0x03;
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

void _lsr(char* path)
{	
	int i=0;
	char spl[MAXFILES][MAXNAME];
	split(path, '/', spl);
	fileTree_t* node = getNode(spl);
	for(i=0; i<node->cantChilds; i++)
	{
		char color, old;
		fileEntry_t entry=ENTRY(node->childs[i]->index);
		if(entry.del){
			old=process[current].tty->color;
			switch(entry.type){
				case DIR:
					color=0x0A;
					break;
				case LINK:
					color=0x05;
					break;
				case FILE:
					color=0x03;
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
	
	fileTree_t *linked = getNode(path);
	if(linked==0){
		return -2;
	}
	fileTree_t *newLink=mallocFS(sizeof(fileTree_t));
	
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
		freeFS(newLink);
		return -2;
	}
	if(alreadyExists(linkName, dad)){
		freeFS(newLink);
		return -5;
	}
	dad->childs[dad->cantChilds++]=newLink;
	newLink->parent=dad;
	newLink->type=LINK;
	writeFile(newLink,0,0);
	//freeNode(tree);
	//loadTree();
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
			if(isStr){
				removeChild(node);
				freeNode(node);
			}
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
	if(nodeT==0 || nodeF==0){
		return -2;
	}
	if(alreadyExists(newName, nodeT)){
		return -5;
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
	inode_t inode;
	open(nodeF, &inode);
	void* buffer = mallocFS(inode.size);
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
	
	fileTree_t* dad=getNode(pathTo);
	if(dad==0){
		return -2;
	}
	if(alreadyExists(nodeName, dad)){
		return -5;
	}
	fileTree_t* newNode=mallocFS(sizeof(fileTree_t));
	//newNode->inode=nodeF->inode;
	newNode->del=0;
	newNode->parent=dad;
	newNode->type=nodeF->type;
	strcpy(newNode->name, nodeName);
	newNode->cantChilds=nodeF->cantChilds;
	dad->childs[dad->cantChilds++]=newNode;
	if(isChildOf(newNode, nodeF)){
		freeFS(newNode);
		return -7;
	}
	cpyChilds(nodeF, newNode);
	if(nodeF->type!=DIR){
		inode_t inode;
		open(nodeF, &inode);
		void* buffer=mallocFS(inode.size);
		readAll(&inode, &buffer);
		writeFile(newNode, buffer, inode.size);
		freeFS(buffer);
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
	if(alreadyExists(nodeName, dad)){
		return -5;
	}
	fileTree_t* newNode=mallocFS(sizeof(fileTree_t));
	newNode->del=0;
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
	if(node->type==DIR || (node->type==LINK && ENTRY(ENTRY(node->index).linkTo).type!=FILE)){
		return -11;
	}
	//inode_t in;
	//ata_read(ATA0, (void*)&in, 512, table[node->index].inode, 0);
	void * buffer=mallocFS(512);
	inode_t inode;
	open(node, &inode);
	int i=0;
	while(i!=MAXSECTOR && inode.sector[i]!=-1){
		//printf("sector %d: %d, \n", i, inode.sector[i]);
		read(&inode, i++, &buffer);
		printf("%s\n", (char*)buffer);
	}
	freeFS(buffer);
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
	buffer=mallocFS(inode.size);
	readAll(&inode, &buffer);
	memcpy(buffer+(inode.size-len), string, len);
	writeSnap(node, buffer, inode.size);
	freeFS(buffer);
	return 0;
}



int revertLast(char* file){
	return revertTo(file, 1);
}



int revertTo(char* file, int version){
	if(version == 0){
		return 0;
	}
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}

	fileEntry_t previous = ENTRY(node->index);
	int oldIndex=node->index;
	int i = 0, j= node->index;
	while(i!=version){
		if(previous.prev==-1){
			return -13;
		}
		j=previous.prev;
		previous = ENTRY(j);
		i++;
	}
	
	fileTree_t * dad = getParentFromTable(&previous);
	if(dad==0){
		return -2;
	}
	if(alreadyExists(previous.name, dad)){
		return -18;
	}
	removeChild(node);
	freeNode(node);

	node = complete(dad, j);
	inode_t inode;
	open(node, &inode);
	void* buffer=mallocFS(inode.size);
	readAll(&inode, &buffer);
	node->index=oldIndex;
	writeSnap(node, buffer, inode.size);
	freeFS(buffer);
	return 0;
}

int _cd(char* path){
	char realPath[MAXFILES][MAXNAME];
	split(path, '/', realPath);
	fileTree_t* node=getNode(realPath);
	if(node==0 || node->del){
		return -2;
	}
	if(node->type==FILE || (node->type==LINK && ENTRY(ENTRY(node->index).linkTo).type!=DIR)){
		return -14;
	}
	CWD=node;
	return 0;
}


int bigFile(char* file){
	char buffer[512], c='a';
	int i=MAXSECTOR, j, z=0, index, s;
	fileEntry_t entry = getFreeEntry(&index);
	char path[MAXFILES][MAXNAME],nodeName[MAXNAME];
	split(file, '/', path);
	setLastStr(path, nodeName);
	fileTree_t* dad=getNode(path);
	if(dad==0){
		return -2;
	}
	if(alreadyExists(nodeName, dad)){
		return -5;
	}
	fileTree_t* newNode=mallocFS(sizeof(fileTree_t));
	newNode->del=0;
	strcpy(newNode->name, nodeName);
	newNode->type=FILE;
	newNode->parent=dad;
	dad->childs[dad->cantChilds++]=newNode;
	newNode->index=index;
	strcpy(entry.name, nodeName);
	entry.type=FILE;
	entry.del=0;
	entry.free=0;
	entry.prev=-1;
	entry.next=-1;
	entry.parent=newNode->parent->index;
	sys_hour(&entry.hour);
	sys_min(&entry.min);
	inode_t inode;
	inode.size=512*i;

	for(j=0; j<i;j++){
		for(z=0; z<511; z++){
			buffer[z]=c;
		}
		buffer[z]=0;
		c++;
		s = getSector();
		if(s==-1){
			return -66;
		}
		inode.sector[j]=s;
		ata_write(ATA0, buffer, 512, j,0);
	}
	for(; j<MAXSECTOR; j++){
			inode.sector[j]=-1;
	}
	s=getSector();
	if(s==-1){
		return -66;
	}
	entry.inode=s;
	writeInode(&entry, &inode);
	writeEntry(index, &entry);
	return 0;
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
	printVersion(&entry, i++);
	return 0;
}

void printVersion(fileEntry_t * entry, int index){
	printf("Version: %d, name: %s, parent: %s, time: %d:%d\n", index, entry->name, entry->parent==-1?"root":(ENTRY(entry->parent).name), entry->hour, entry->min);
}	


