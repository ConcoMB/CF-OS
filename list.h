typedef struct listCDT *listADT;

/*FUNCTIONS*/

/*Creates Header*/
listADT newList (int bytes, int (*function)(void*, void*));

/*Create node and insert element*/
void insertElem( listADT header, void *newElem);

/*Deletes node*/
void deleteElem(listADT header, void*delElem);

/*Returns a copy of the element in the head*/
void * listHead ( listADT header);

/*Searches for de element in pos and prints it*/
void getElem(listADT header, int pos,void (*printType)(void*));

/*Returns the tail of the actual list*/
listADT listTail ( listADT header);

void freeList( listADT header);

listADT intersection (listADT header1, listADT header2);

listADT unionList ( listADT header1, listADT header2);

void reset(listADT list);

void* getNext(listADT list);

