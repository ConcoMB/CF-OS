#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILES 100
#define MAXSIZE 4096
#define MAXPATH 100
#define MAXSECTOR 10
#define MAXNAME 20
#define MAXSNAPSHOTS 15

#define SET(n) (bitMap[n/8]|=0x01<<(n%8))
#define GET(n) ((bitMap[n/8]>>(n%8))&0x01)
#define FREE(n) (bitMap[n/8]&= ~(0x01<<(n%8)))
typedef enum {DIR, F, LINK} fileType_t;

typedef struct{
	int sector[MAXSECTOR];
	int size;
}inode_t;

typedef struct fileEntry_t{
	char name[MAXNAME];
	int parent;
	inode_t inode;
	//struct fileEntry_t snapshots[MAXSNAPSHOTS];
	fileType_t type;
	char free;		
}fileEntry_t;


typedef struct{
	fileEntry_t files[MAXFILES];
}fileTable_t;

int sectorIndex;
char bitMap[MAXFILES * MAXSIZE / (512*8)];

typedef struct fileTree_t{
	struct fileTree_t* childs[MAXFILES];
	int cantChilds;
	struct fileTree_t* parent;
	char name[MAXNAME];
	inode_t inode;
	fileEntry_t snapshots[MAXSNAPSHOTS];
	fileType_t type;
}fileTree_t;

fileTree_t* tree;
fileTable_t table;



void fill(fileTree_t* tree, fileTable_t* table, int myEntry)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(!table->files[i].free && table->files[i].parent==myEntry){
			fileTree_t* son=malloc(sizeof(fileTree_t));
			fileEntry_t entry= table->files[i];
			strcpy(son->name, entry.name);
			son->type=entry.type;
			//son->snapshots=entry.snapshots;
			son->inode=entry.inode;
			son->parent=tree;
			son->cantChilds=0;
			tree->childs[tree->cantChilds++]=son;
			if(entry.type==DIR){
				fill(son, table, i);
			}
		}
	}
}
void loadTree(){
	tree = malloc(sizeof(fileTree_t));
	strcpy(tree->name,"/");
	tree->cantChilds=0;
	tree->parent=tree;
	tree->type=DIR;

	fill(tree, &table, -1);
}



printTree(fileTree_t* aTree){
	printf("name %s, parent %s\n", aTree->name, aTree->parent->name);
	int i;
	for(i=0;i<aTree->cantChilds; i++){
		printTree(aTree->childs[i]);
	}
}

int main(){
	int i;
	for(i=0; i<MAXFILES; i++){
		table.files[i].free=1;
	}
	table.files[1].free=0;
	strcpy(table.files[1].name,"proc1");
	table.files[1].parent=-1;
	table.files[1].type=DIR;
	table.files[2].free=0;
	strcpy(table.files[2].name,"proc1.1");
	table.files[2].type=F;
	table.files[2].parent=1;
	table.files[10].free=0;
	strcpy(table.files[10].name,"HOLA");
	table.files[10].parent=-1;
	table.files[10].type=DIR;

	loadTree();

	printTree(tree);
}
