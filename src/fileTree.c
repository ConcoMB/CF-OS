#include "../include/fileTree.h"

void printTree(fileTree_t* aTree){
	char* type;
	switch(aTree->type){
		case DIR:
			type="DIR";
			break;
		case FILE:
			type="FILE";
			break;
		case LINK:
			type="LINK";
			break;
	}
	printf("name %s, type %s, parent %s\n", aTree->name, type, aTree->parent->name);
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
	tree->index=-1;
	tree->type=DIR;
	int i;
	for(i=0; i<8; i++){
		cwd[i]=tree;
	}
	fill(tree, -1);
}

void fill(fileTree_t* tree, int myEntry)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(!table.files[i].free && table.files[i].parent==myEntry && table.files[i].next==-1 && !table.files[i].del){
			fileTree_t* son=malloc(sizeof(fileTree_t));
			fileEntry_t entry= table.files[i];
			strcpy(son->name, entry.name);
			son->type=entry.type;
			//son->snapshots=entry.snapshots;
			//son->inode=entry.inode;
			son->parent=tree;
			son->cantChilds=0;
			tree->childs[tree->cantChilds++]=son;
			son->index=i;
			if(entry.type==DIR){
				fill(son, i);
			}
		}
	}
}

void complete(fileTree_t* dad, int index){
	fileTree_t* son=malloc(sizeof(fileTree_t));
	fileEntry_t entry= table.files[index];
	strcpy(son->name, entry.name);
	son->type=entry.type;
	son->parent=dad;
	son->cantChilds=0;
	dad->childs[dad->cantChilds++]=son;
	son->index=index;
	if(entry.type==DIR){
		fill(son, index);
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

void freeNode(fileTree_t* node){
	int i;
	for(i=0; i<node->cantChilds; i++){
		freeNode(node->childs[i]);
	}
	free(node);
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

void clone(fileTree_t* cloned, fileTree_t* new){
	strcpy(new->name, cloned->name);
	new->cantChilds=cloned->cantChilds;
	new->parent=cloned->parent;
	new->type=cloned->type;
	if(cloned->type!=DIR){
		inode_t inode;
		open(cloned, &inode);
		void* buffer=malloc(inode.size);
		readAll(&inode, &buffer);
		writeFile(new, buffer, inode.size);
		//free(buffer);
	}else{
		writeFile(new, 0,0);
	}
}

void cpyChilds(fileTree_t* from, fileTree_t* to)
{
	int i;

	for(i=0; i<from->cantChilds; i++)
	{
		fileTree_t* new = malloc(sizeof(fileTree_t));
		to->childs[i]=new;
		clone(from->childs[i], new);
		if(from->childs[i]->type==DIR){
			cpyChilds(from->childs[i], to->childs[i]);
		}
	}
}

void lnChilds(fileTree_t* from, fileTree_t* to)
{
	int i;
	for(i=0; i<from->cantChilds; i++)
	{
		to->childs[i]=from->childs[i];
	}
}

void buildPath(char path[], fileEntry_t* entry){
	
	while(entry->parent!=-1){
		strcat(path,entry->name);
		entry=&(table.files[entry->parent]);
	}
}

fileTree_t* getNode(char path[][MAXNAME])
{
	int i=0;
	fileTree_t* myTree;
	if(strcmp(path[i], "..")==0){
		do{
			myTree=CWD->parent;
		}while(strcmp(path[++i],"..")==0);
	}else if(strcmp(path[0], ".")==0) {
		myTree=CWD;
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
	char nodes[MAXFILES][MAXNAME];
	split(parent, '/', nodes);
	setParentW(newTree, nodes, 0, tree);
}

int alreadyExists(char* name){
	fileTree_t* thisTree = CWD;
	int i;
	for(i=0; i<thisTree->cantChilds; i++){
		if(strcmp(name, thisTree->childs[i]->name)==0){
			return 1;
		}
	}
	return 0;
}

