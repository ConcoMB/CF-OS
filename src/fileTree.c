#include "../include/fileTree.h"

fileTree_t* tree;
fileTable_t table;


void loadTree(){
	tree = malloc(sizeof(fileTree_t));
	strcpy(tree->name,"/");
	tree->cantChilds=0;
	tree->parent=tree;
	tree->type=DIR;

	fill(tree, &table, -1);
}

void fill(fileTree_t* tree, fileTable_t* table, int myEntry)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(!table->files[i].free && table->files[i].parent==myEntry){
			fileTree_t* son=malloc(sizeof(fileTree_t));
			fileEntry_t entry= table->files[i];
			strcpy(son->name, entry.name);
			son->type=entry.type;
			son->snapshots=entry.snapshots;
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

