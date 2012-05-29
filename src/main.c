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


char **split ( char *string, const char sep, char list[MAXNAME][MAXFILES]) 
{
    char *p = string;
    int  i = 0;
    int  pos=0;
    const int len = strlen (string);
    list[0][0] = '\0';
    while (i <len) {
        while ((p[i] == sep) && (i <len))
            i++;
        if (i <len) {
       
            list[pos + 1][0] = '\0';
            int j = 0;
            for (i; ((p[i] != sep) && (i <len)); i++) {
                list[pos][j] = p[i];
                j++;
            }
            list[pos][j] = '\0';
            pos++;
        }
    }
}
void fill(fileTree_t* myTree, fileTable_t* table, int myEntry)
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
			son->parent=myTree;
			son->cantChilds=0;
			myTree->childs[myTree->cantChilds++]=son;
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
void setParentW(fileTree_t* newTree, char nodes[MAXNAME][MAXFILES], int index, fileTree_t* thisTree)
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
	/*int sector = getSector();
	if(sector==-1){
		//error
		return -1;
	}
	entry.inode.sector[0]=sector;
	*///ESCRIBIR EN EL DISCO
	return 0;
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
	_mkdir("dir", "/proc1");
	printTree(tree);
}
