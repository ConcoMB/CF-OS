
fileTree_t* tree;

typedef struct{
	fileTree_t* childs[MAXFILES];
	int cantChilds;
	fileTree_t* parent;
	char name[MAXNAME];
	inode_t inode;
	fileEntry_t snapshots[MAXSNAPSHOTS];
	fileType_t type;
}fileTree_t;

void loadTree(){
	//read
	fileTable_t table;
	tree = malloc(sizeof(fileTree_t));
	tree->name="/";
	tree->cantChilds=0;
	tree->parent=tree;
	tree->type=DIR;

	fill(tree, &table, -1);
}

void fill(fileTree_t* tree, fileTable_t* table, int myEntry)
{
	int i;
	for(i=0; i<MAXFILES; i++){
		if(!table[i].free && table[i].parent==myEntry){
			fileTree_t son=malloc(sizeof(fileTree_t));
			fileEntry_t entry= table[i];
			son->name=entry.name;
			son->type=entry.type;
			son->snapshots=entry.snapshots;
			son->inode=entry.inode;
			son->parent=tree;
			son->cantChilds++;
			tree->childs[tree->cantChilds++]=son;
			if(entry.type==DIR){
				fill(son, table, entry);
			}
		}
	}
}

