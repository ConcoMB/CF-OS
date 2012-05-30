#include "../include/fileTree.h"

fileTree_t* tree, *current;
fileTable_t table;

printTree(fileTree_t* aTree){
	printf("name %s, parent %s\n", aTree->name, aTree->parent->name);
	int i;
	for(i=0;i<aTree->cantChilds; i++){
		printTree(aTree->childs[i]);
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
	fileTree_t* myTree;
	if(strcmp(path[i], "..")==0){
		do{
			myTree=current->parent;
		}while(strcmp(path[++i],"..")==0);
	}else if(strcmp(path[0], ".")==0) {
		myTree=current;
		i++;
	}else{
		//path absoluto;
		myTree=tree;
	}
	while(path[i][0]!='\0'){
		int j, flag=0;
		for(j=0; j<myTree->cantChilds && flag!=1; j++){
			if(strcmp(myTree->childs[j]->name, path[i])==0){
				myTree=myTree->childs[j];
				flag=1;
			}
		}
		if(flag==0){
			return 0;
		}
		i++;
	}
	return myTree;
}


int isChildOf(fileTree_t* dad, fileTree_t* son)
{
	fileTree_t* iter=son;
	while(iter!=tree){
		if(iter->parent==dad){
			return 1;
		}
		iter=iter->parent;
	}
	return 0;
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

