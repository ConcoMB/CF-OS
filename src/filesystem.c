#include "../include/filesystem.h"
extern fileTree_t* tree;
extern fileTable_t table;

int getSector()
{
	int i;
	int c = 0, end = MAXFILES * MAXSIZE / (512*8);
	for(i=sectorIndex; c!=end; i++, c++){
		if(i==(MAXFILES * MAXSIZE / (512*8))){
			i=0;
		}
		if(GET(i)==0){
			SET(i);
			sectorIndex=i;
			writeBitMap();
			return i;
		}
	}
	return -1;
}

int fileSyst(int argc, char** argv){

	initializeFS();

	getStackPage(current);
	getStackPage(current);
	int i;
	for(i=0; i<MAXFILES; i++){
		table.files[i].free=1;
	}

	readTable();
	readBitMap();
	loadTree(tree);
	_touch("touch");
	_ln("touch", "link");
	attatch("touch", "tea");
	_cat("link");
	_ln("link", "link2");
	attatch("link2", "hola");
	_cat("link");
	printf("TREE\n");
	printTree(tree);
	printTable();
	
	return 0;
}

void printTable(){
	int i;
	printf("TABLE\n");
	for(i=0; i<MAXFILES; i++){
		fileEntry_t e = ENTRY(i);
		if(!e.free){
			printf("%s , %s %s\n", e.name, e.next==-1?"ultima version":"version vieja", e.del==1?" borrado logico":"");
		}
	}
}
fileEntry_t getFreeEntry(int* index)
{
	int i=0;
	for(i=0; i<MAXFILES; i++)
	{
		if(ENTRY(i).free){
			*index = i;
			return ENTRY(i);
		}
	}
	//no hay lugar
	return ENTRY(i);

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



int snapList(char* file){
	char path[MAXFILES][MAXNAME];
	split(file, '/', path);
	fileTree_t* node = getNode(path);
	if(node==0){
		return -2;
	}
	int i=0;
	printf("Revision of file %s\n", file);
	fileEntry_t entry = ENTRY(node->index);
	while(entry.prev!=-1){
		printf("Version %d: name %s, parent name %s, inode sector %d\n", i++, entry.name, ENTRY(entry.parent).name, entry.inode);
		entry=ENTRY(entry.prev);
	}
	printf("Name %s, parent name %s, inode sector %d\n", entry.name, ENTRY(entry.parent).name, entry.inode);
	return 0;	
}


fileTree_t* getParentFromTable(fileEntry_t* entry){
	char path[MAXPATH], this[MAXNAME];
	path[0]='\0';
	buildPath(path, entry);
	char splPath[MAXFILES][MAXNAME];
	split(path, '/', splPath);
	setLastStr(splPath, this);
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

void initializeFS(){
	_Cli();
	initTable();
	initBitMap();
	readTable();
	readBitMap();
	loadTree(tree);
	_Sti();
}

void initBitMap(){
	int pos = sizeof(fileEntry_t)*MAXFILES/512;
	pos++;
	int i;
	for(i=0; i<=pos; i++){
		SET(i);
	}
	writeBitMap();
}

void printBitMap(){
	int i;
	printf("BITMAP\n");
	for(i=0; i<MAXFILES * MAXSIZE / (512*8); i++){
		if(GET(i)){
			printf("%d, ", i);
		}
	}
}

void initTable(){
	fileTable_t tab;
	int i;
	for(i=0; i<MAXFILES; i++){
		tab.files[i].free=1;
	}
	ata_write(ATA0, tab.files, sizeof(fileEntry_t)*MAXFILES, 0,0);

}

void readTable(){
	ata_read(ATA0, table.files, sizeof(fileEntry_t)*MAXFILES, 0,0);
}

void open(fileTree_t* node, inode_t* inode){
	fileEntry_t e;
	if(node->type==LINK){
		e = ENTRY(ENTRY(node->index).linkTo);
	}else{
		e= ENTRY(node->index);
	}
	while(e.next!=-1){
		e= ENTRY(e.next);
	}
	openEntry(&e, inode);
	/**buffer=malloc(inode.size*512);
	int i;
	for(i=0; i<inode.size; i++){
		ata_read(ATA0, (*buffer)+i*512, 512, inode.sector[i],0);
	}*/
}
void openEntry(fileEntry_t* e, inode_t* inode){
	if(e->inode!=-1){
		ata_read(ATA0, (void*)inode, sizeof(inode_t), e->inode,0);
	}
	else
	{
		inode->size=0;
		int i;
		for(i=0; i<MAXSECTOR; i++){
			inode->sector[i]=-1;
		}
	}
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
		//printf("i:%d\n",i);
		size=512;
		if(i!=MAXSECTOR && inode->sector[i+1]==-1){
			size=inode->size%512;
		}
		ata_read(ATA0, *buffer+i*512, size, inode->sector[i], 0);
		i++;
	}
}

void write(inode_t* inode, int which, int size, void* buffer){
	ata_write(ATA0, buffer, size, inode->sector[which], 0);
}

void create(fileEntry_t* entry, void* buffer, int size, int index){
	if(size!=0){
		int sects = size/512;
		size%512!=0?sects++:1;
		inode_t inode;
		inode.size=size;
		int i, j;
		for(i=0; i<sects; i++){
			j=getSector();
			inode.sector[i]=j;
			if(sects-1 == i){
				ata_write(ATA0, buffer+i*512, size%512, j,0);
			}
			else
				ata_write(ATA0, buffer+i*512, 512, j,0);
		}
		for(; i<MAXSECTOR; i++){
			inode.sector[i]=-1;
		}
		int inodeSect=getSector();
		entry->inode=inodeSect;
		writeInode(entry, &inode);
	}else{
		entry->inode=-1;
	}
	writeEntry(index, entry);
}

void writeSnap(fileTree_t* node, void* buffer, int size){
	int newIndex, oldIndex=node->index;
	fileEntry_t old= ENTRY(oldIndex);
	fileEntry_t entry=getFreeEntry(&newIndex);
	old.next=newIndex;

	writeEntry(oldIndex, &old);

	strcpy(entry.name, node->name);
	entry.type=node->type;
	entry.parent=node->parent->index;
	entry.free=0;
	if(size==-5)
	{
		entry.del=1;
		size=0;
	}else{
		entry.del=0;
	}
	entry.prev=oldIndex;
	entry.next=-1;
	create(&entry, buffer, size, newIndex);
	node->index=newIndex;

}

void writeFile(fileTree_t* node, void* buffer, int size){
	int i;
	fileEntry_t entry=getFreeEntry(&i);
	if(node->type==LINK){
		entry.linkTo=node->index;
		while(ENTRY(entry.linkTo).type==LINK){
			entry.linkTo=ENTRY(entry.linkTo).linkTo;
		}
	}
	node->index=i;
	entry.next=-1;
	entry.prev=-1;
	entry.type=node->type;
	
	entry.parent=node->parent->index;
	entry.free=0;
	entry.del=0;
	strcpy(entry.name, node->name);
	create(&entry, buffer, size, i);
}
/*
void snapCP(fileTree_t* node){
	int i;
	fileEntry_t entry=getFreeEntry(&i);
	fileEntry_t old= ENTRY(node->index);
	old.next=i;
	entry.prev=node->index;
	entry.next=-1;
	entry.inode=old.inode;
	writeEntry(node->index, &old);
	node->index=i;
	writeEntry(i, &entry);
	printf("nueva entrada en %d, a vieja era %d, inodo %d\n", i, entry.prev, entry.inode);
}
*/
void delFile(fileTree_t* node, char isStr){
	int i;
	if(!isStr){
		writeSnap(node, 0, -5);
		node->del=1;
	}else{
		i=node->index;
		fileEntry_t entry = ENTRY(i);
		while(entry.prev!=-1){
			entry.free=1;
			writeEntry(i, &entry);
			FREE(i);
			i=entry.prev;
			entry=ENTRY(entry.prev);
		}
		entry.free=1;
		FREE(i);
		writeEntry(i, &entry);
		writeBitMap();
	}
	
}
/*
int getFile(fileTree_t* node)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(strcmp(node->name, ENTRY(i).name)==0){
			while(ENTRY(i).next!=-1){
				i=ENTRY(i).next;
			}
			return i;
		}
	}
	return -1;
}*/

void writeEntry(int index, fileEntry_t* entry){
	table.files[index]=(*entry);
	int pos = index*sizeof(fileEntry_t);
	ata_write(ATA0, (void*)&ENTRY(index), sizeof(fileEntry_t), pos/512, pos%512);
}

void writeInode(fileEntry_t* entry, inode_t* inode)
{
	ata_write(ATA0, (void*)inode, sizeof(inode_t), entry->inode, 0);
}

void writeBitMap(){
	int pos = sizeof(fileEntry_t)*MAXFILES/512;
	pos++;
	ata_write(ATA0, (void*)&bitMap, sizeof(bitMap), pos, 0);
}

void readBitMap(){
	int pos = sizeof(fileEntry_t)*MAXFILES/512;
	pos++;
	ata_read(ATA0, (void*)&bitMap, sizeof(bitMap), pos, 0);
}

int FSServer(int a, char** v){
	getStackPage(current);
	getStackPage(current);

	readTable();
	readBitMap();
	loadTree(tree);
	int i;
	for(i=0; i<8; i++){
		cwd[i]=tree;
	}
	msg_t msg;
	while(1){
		msgRead(&msg);
	}
	return 0;
}

void freeInodes(fileEntry_t* entry){
	int i=0;
	inode_t inode;
	openEntry(entry, &inode);
	while(inode.sector[i]!=-1){
		FREE(inode.sector[i]);
		i++;
	}
	FREE(entry->inode);
}
