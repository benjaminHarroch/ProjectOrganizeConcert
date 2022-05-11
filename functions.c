
//לא לשכח לבדוק בסוף הפרוייקט את כל האלוקציות במקות
//ולא לשכח לשחרר זיכרון של כל מה שצריך לשחרר
//לא לשכח לסגור קבצים
//לא לשכח לשים לב לDEFINE של CONST
#include "Header.h"


void buildBinaryTreeFromFile(char* filename, InstrumentTree *tr) {

	char* instrument;
	int size,flage;
	unsigned short id=0;
	flage = 0;

	FILE* fp;
	fp = fopen(filename, "r");
	chekeIfOpen(fp);

	instrument=getTheInstrument(fp);
	tr->root = creatTreeNode(instrument, &id, NULL, NULL);
	
	while (instrument != NULL) {

		instrument=getTheInstrument(fp);
		helperBuild(tr->root, instrument,&flage,&id);
		flage = 0;
	}
	
	fclose(fp);
}

TreeNode* creatTreeNode(char *instrument, unsigned short *insID, TreeNode *left, TreeNode *right) {

	int size;
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	chekeAlloction(newNode);

	newNode->insID = *insID;
	size = strlen(instrument);
	newNode->instrument = (char*)malloc(size+1);
	strcpy(newNode->instrument, instrument);
	newNode->instrument[size + 1] = '\0';

	newNode->left = left;
	newNode->right = right;

	(*insID)++;

	return newNode;
}

//FILE with names- dont forget to add '\0' to the end of the name
// and use the fscanf function for flage 
//when the function success to scan on name return 1 if he dont success to scan a name so he return -1
char* getTheInstrument(FILE* fp) {

	char* instrument = (char*)malloc(sizeof(char)* MAX_INSTRUMENT_NAME);
	chekeAlloction(instrument);
	int flage,sizeOfname;

	flage=fscanf(fp, "%s", instrument);
	
	if ( flage == -1 ){

		return NULL;
	}
	
	sizeOfname = strlen(instrument);
	instrument[sizeOfname+1] = '\0';

	instrument = (char*)realloc(instrument, (sizeOfname+1));
	
	return instrument;
}

void chekeIfOpen(FILE *fp) {
	
	if (fp == NULL) {
		printf("the file no success to open");
	}

}

void chekeAlloction(void* ptr) {

	if (ptr == NULL) {
		printf("the allocation is failed.");
		exit(-1);
	}
}

void helperBuild(TreeNode *root,char* instrument,int *flage,int *id) {

	int numCompare;
	if (root == NULL || instrument == NULL) {

		return;
	}
	else {

		numCompare = strcmp(instrument, root->instrument);

		if (numCompare >0) {

			helperBuild(root->right, instrument, flage,id);

			if (*flage == 0) {
				root->right = creatTreeNode(instrument, id, NULL, NULL);
				*flage = 1;
			}
		}else if (numCompare < 0) {

			helperBuild(root->left, instrument, flage, id);

			if (*flage == 0) {
				root->left = creatTreeNode(instrument, id, NULL, NULL);
				*flage = 1;
			}
		}
		else {
			return;
		}
		
	}
}

void freeTree(InstrumentTree tr) {

	FreeTreehelper(tr.root);
}

void  FreeTreehelper(TreeNode* root) {

	if (root == NULL) {
		return;
	}
	else {

		FreeTreehelper(root->left);
		FreeTreehelper(root->right);
		free(root->instrument);
		free(root);
	}
}

void helperToPrint(TreeNode* root) {

	if (root == NULL) {
		return;
	}
	else {
		//pritn all the left side and the root and the right side.
		helperToPrint(root->left);
		printf("%s ", root->instrument);
		printf("%d ", root->insID);
		helperToPrint(root->right);
	}

}

void printTree(InstrumentTree tr) {

	helperToPrint(tr.root);

}

//purpose: search the ID of an instrument
//if not find return NULL if find return the ID
int findInsId(InstrumentTree tree, char* instrument){

	int flage;
	flage = 0;
	int id;
	helperFindInsId(tree.root, instrument,&flage,&id);

	if (flage == 0) {
		return NOT_FOUND;
	}else{
		return id;
	}

}
void helperFindInsId(TreeNode *root,char* instrument,int *flage,int *id) {

	if (root == NULL || *flage == 1) {
		return;
	}
	else {

		helperFindInsId(root->left, instrument, flage,id);
		helperFindInsId(root->right, instrument, flage,id);

		if (strcmp(instrument, root->instrument) == 0) {
			*flage = 1;
			*id = root->insID;
		}

	}
}

//go over the file 
//for every line save the data and count how many i have singer.
//the amount will be the size of my array of musician
void saveMusicianFromFile(char* filename, InstrumentTree* tr) {

	FILE* fp;
	fp = fopen(filename, "r");
	chekeIfOpen(fp);
	char line[150];
	char* ptr;
	int psize, lsize, lsizeForMusicianArray, psizeForMusicianArray, lsizeName = 0;
	char** name;
	lsizeForMusicianArray = 0;
	psizeForMusicianArray = 2;

	Musician** musicianGroup;
	musicianGroup = (Musician**)malloc(sizeof(Musician*) * psizeForMusicianArray);

	ptr = fgets(line, 150, fp);

	//this while run on every line in the file per one line every time
	while (ptr != NULL) {

		if (lsizeForMusicianArray == psizeForMusicianArray) {
			psizeForMusicianArray = psizeForMusicianArray * 2;
			musicianGroup = (Musician**)realloc(musicianGroup, psizeForMusicianArray * (sizeof(Musician*)));
		}

		musicianGroup[lsizeForMusicianArray] = (Musician*)malloc(sizeof(Musician));
		makeEmptyList(&(musicianGroup[lsizeForMusicianArray]->instrument));

		name = buildNameAndList(&musicianGroup, tr, lsizeForMusicianArray, line, &lsizeName);

		musicianGroup[lsizeForMusicianArray]->name = name;
		musicianGroup[lsizeForMusicianArray]->chozen = 0;
		musicianGroup[lsizeForMusicianArray]->lenghtName = lsizeName;
		lsizeName = 0;
		lsizeForMusicianArray++;
		ptr = fgets(line, 150, fp);
	}
	fclose(fp);
	buildArrayOfMusicianCollection(musicianGroup, tr, lsizeForMusicianArray);
}

void makeEmptyList(MPIList* lst) {

	lst->head = lst->tail = NULL;
}

void insertNodeToTail(MPIList* lst, ListNode* cur) {


	if (lst->head==NULL&&lst->tail == NULL) {

		lst->head = lst->tail = cur;
	}
	else {
		cur->next = NULL;
		lst->tail->next = cur;
		lst->tail = cur;
	}
}

void creatNodeAndInsertToTail(MPIList* lst,ListNode *next, unsigned short insID,float price) {

	ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
	chekeAlloction(newNode);

	newNode->next = next;
	newNode->mpi.insID = insID;
	newNode->mpi.price = price;

	insertNodeToTail(lst, newNode);
	
}


char **buildNameAndList(Musician ***musicianGroup,InstrumentTree *tr,int lsizeForMusicianArray,char *line,int *lsizeName) {

	int id,thisIsaName,lsize,psize,nameLenght,saveID;
	char* specialChar = " ,.;:?!-\t'()[]{}<>~_\n";
	char* token;
	thisIsaName = 0;
	psize = 2;

	char** name;
	name = (char**)malloc(sizeof(char*) * psize);

	/* get the first token */
	token = strtok(line, specialChar);

	/* walk through other tokens */
	while (token != NULL) {

		//get the id of the different instruments
		id = findInsId(*tr, token);

		//the name is in the begine so every token at the begining is a part of the name
		//ang when i arrive to a instrument i change the fkage beacaus
		//from now the token that are without id number is a *number*!!
		if (id == -1 && thisIsaName == 0) {

			if (*lsizeName == psize) {
				psize = psize * 2;
				name = (char**)realloc(name, psize * (sizeof(char*)));
			}

			nameLenght = strlen(token);
			name[*lsizeName] = (char*)malloc(sizeof(char) * (nameLenght + 1));
			strcpy(name[*lsizeName], token);
			name[*lsizeName][nameLenght + 1] = '\0';
			(*lsizeName)++;

		}else {
			thisIsaName = 1;
			if (id == -1 && token != "\n") {

				creatNodeAndInsertToTail(&((*musicianGroup)[lsizeForMusicianArray]->instrument), NULL, saveID, atoi(token));
			}
			saveID = id;
		}
		//strtok continue on the same line if you put NULL 
		token = strtok(NULL, specialChar);
	}
	return name;
}

//the amount of  musician describe the size of the array group 
//this function will over on the array of instrument and add
//every musician to the appropriate place 
void buildArrayOfMusicianCollection(Musician** musicianGroup, InstrumentTree* tr, int size) {

	int numOfInstrument = getNumOfInstrument(tr);
	int* arrOfSize = (int*)malloc(sizeof(int) * numOfInstrument);
	Musician*** musicianCollection = (Musician***)malloc(sizeof(Musician**) * numOfInstrument);

	int id;

	for (id = 0; id < numOfInstrument; id++) {

		musicianCollection[id] = getArrayOfMusicianWhoPlayTheSameInstrument(musicianGroup, id, size,arrOfSize);
		
	}
	getConcertFromTheUserAndShowConcert(*tr,musicianCollection,numOfInstrument,arrOfSize);
}

//purpose: get the num of instrument in all the file
//because my tree compose all the instrument i can run over the tree
//the amount of nodes in the tree is the amount of instrument.
int  getNumOfInstrument(InstrumentTree* tr) {

	int counter = 0;
	helperGetNumsOfInstrument(tr->root, &counter);
	return counter;

}

void helperGetNumsOfInstrument(TreeNode *root,int *counter) {

	if (root == NULL) {
		return;
	}
	else {
		helperGetNumsOfInstrument(root->left,counter);
		helperGetNumsOfInstrument(root->right, counter);
		(*counter)++;
	}
}

//3 different argument
//1-the array of musician group
//2- the id who i search for choose the singer who use this instrument(id)
// the size of the array of musician group
//return an array of pointer to musician with the same instrument
Musician** getArrayOfMusicianWhoPlayTheSameInstrument(Musician **musicianGroup, int id,int size,int *arrofsize) {

	int lsizeForMusicianArray, psizeForMusicianArray,i;
	lsizeForMusicianArray = 0;
	psizeForMusicianArray = 2;

	Musician** musicianGroupofTheSameInstrument;
	musicianGroupofTheSameInstrument= (Musician**)malloc(sizeof(Musician*) * psizeForMusicianArray);

	//loop that run one the musician group for search the ID on the array
	//every one who have this ID one this array so a get him for my new array
	for (i = 0; i < size; i++) {

		Musician* tmp;

		if (lsizeForMusicianArray == psizeForMusicianArray) {
			psizeForMusicianArray = psizeForMusicianArray * 2;
			musicianGroupofTheSameInstrument = (Musician**)realloc(musicianGroupofTheSameInstrument, psizeForMusicianArray * (sizeof(Musician*)));
		}

		 tmp=getTheMusicianWhoPlayTheSameInstrument(musicianGroup, i,id);
		
		 //if i found a instrument so add to array and add one place else do nothing
		 if (tmp != NULL) {
			 musicianGroupofTheSameInstrument[lsizeForMusicianArray] = tmp;
			 lsizeForMusicianArray++;
		 }
	} 
	musicianGroupofTheSameInstrument = (Musician**)realloc(musicianGroupofTheSameInstrument, lsizeForMusicianArray * (sizeof(Musician*)));
	arrofsize[id] = lsizeForMusicianArray;

	return musicianGroupofTheSameInstrument;
}

// search for me on the array in place index over the list if the ID exist
//if yes send me back this musician 
Musician* getTheMusicianWhoPlayTheSameInstrument(Musician** musicianGroup,int index, int id) {
	
	MPIList lst = musicianGroup[index]->instrument;
	ListNode* cur = lst.head;
	Musician *musicianGroupofTheSameInstrument = (Musician*)malloc(sizeof(Musician));

	while (cur != NULL) {

		if (cur->mpi.insID == id) {
			
			musicianGroupofTheSameInstrument = musicianGroup[index];
			return musicianGroupofTheSameInstrument;
		}
		cur = cur->next;
	}
	return NULL;
}

//arrofsize-this is an array that save my size of every array 
//example - arrofsize[0] save the size of array musician in place 0 of array collection
void getConcertFromTheUserAndShowConcert(InstrumentTree tr, Musician ***musicianCollection,int numOfInstrument,int *arrOfSize) {
	int lenghtLine,sizeArray=0;
	int* arrOfsizeOfMusicianWhoPlay;
	char* str,*name;
	IDName* arrInstrument;
	Date *date;
	date = (Date*)malloc(sizeof(Date));
	str=getStringe();
	lenghtLine = strlen(str);
	name = (char*)malloc(sizeof(char)*lenghtLine);
	CIList lst;
	Musician ***allMusicianWhoPlayInConcert;

	while (str != NULL) {

		sscanf(str, "%s %d %d %d %f:%f",name,&date->day,&date->month,&date->year,&date->hour,&date->min);
		lenghtLine = strlen(name);
		name = (char*)realloc(name, lenghtLine+1);
		name[lenghtLine + 1] = '\0';

		arrInstrument = getListOfInstrumentInTheConcert(str, tr,&lst);
		arrangeArrayAppropriateToTheImportance(lst, musicianCollection,arrOfSize);

		//array whit number of every musician i need to a concert for veri instrument
		arrOfsizeOfMusicianWhoPlay = getArrOfSizeOfEvryInstrument(lst);

		allMusicianWhoPlayInConcert=arrangeMusicianToConcert(lst, musicianCollection, arrOfSize,&sizeArray,arrOfsizeOfMusicianWhoPlay);

		if (allMusicianWhoPlayInConcert == NULL) {
			printError(name);
		}
		else {
             //allmusicianwhoplayinconcert- this is the new array collection whit the musician
			//sizearray- is my size of this array 
			printTheConcert(allMusicianWhoPlayInConcert,sizeArray,name,date,arrOfsizeOfMusicianWhoPlay, arrInstrument);
		}

		sizeArray = 0;
		str = getStringe();
	}
}


char* getStringe() {
	char* str;
	int logSize = 0, phySize = 1, flage = 0;
	char c;

	str = (char*)malloc(sizeof(char) * phySize);
	chekeAlloction(str);
	c = getchar();

	while (c != '\n')
	{
		if (logSize == phySize)
		{
			phySize *= 2;
			str = (char*)realloc(str, sizeof(char) * phySize);
		}
		str[logSize] = c;
		logSize++;
		c = getchar();	
	}

	str = (char*)realloc(str, sizeof(char) * (logSize +1));
	str[logSize] = '\0';
	flage=chekeIfLineBlanc(str);
	if (flage == 0)
		return NULL;

	return str;
}

int chekeIfLineBlanc(char* str) {
	 
	int i = 0, flage = 0;

	while (str[i] != '\0') {

		if (str[i] != ' ') {
			flage = 1;
			return flage;
		}
		i++;
	}

	return flage;
}

IDName* getListOfInstrumentInTheConcert(char *str,InstrumentTree tr,CIList *alst) {

	CIList lst;
	makeEmptyCIList(&lst);
	int id,nameLen,psize,lsize=0,counter=0;
	psize = 2;
	char* token;
	IDName* arrInstrument = (IDName*)malloc(sizeof(IDName) * psize);
	chekeAlloction(arrInstrument);
	ConcertInstrument* CInstrument=NULL;

	token = strtok(str, " ");

	while (token != NULL) {

		id=findInsId(tr,token);

		if (id != -1 && counter == 0) {

			if (lsize == psize) {
				psize = psize * 2;
				arrInstrument = (IDName*)realloc(arrInstrument,psize*(sizeof(IDName)));
			}

			CInstrument = (ConcertInstrument*)malloc(sizeof(ConcertInstrument));
			chekeAlloction(CInstrument);
			CInstrument->inst = id;
			nameLen = strlen(token);
			
			arrInstrument[lsize].nameOfInstrument= (char*)malloc((nameLen+1) * sizeof(char));
			chekeAlloction(arrInstrument[lsize].nameOfInstrument);
			strcpy(arrInstrument[lsize].nameOfInstrument, token);
			arrInstrument[lsize].nameOfInstrument[nameLen] = '\0';
			arrInstrument[lsize].idOfInstrument = id;
			lsize++;
			counter++;

		}else if (counter == 1) {
			
			CInstrument->num = atoi(token);
			counter++;

		}else if (counter == 2) {

			CInstrument->importance = atoi(token);
			creatNodeAndInsertToCIListTail(&lst, NULL, CInstrument);
			counter = 0;
		}
		token = strtok(NULL, " ");
	}
	
	arrInstrument = (IDName*)realloc(arrInstrument,(sizeof(IDName)*lsize));
	*alst = lst;
	return arrInstrument;
}

void makeEmptyCIList(CIList* lst) {

	lst->head = lst->tail = NULL;
}

void insertNodeToCIListTail(CIList* lst, CIListNode* cur) {


	if (lst->head == NULL && lst->tail == NULL) {

		lst->head = lst->tail = cur;
	}
	else {
		cur->next = NULL;
		lst->tail->next = cur;
		lst->tail = cur;
	}
}

void creatNodeAndInsertToCIListTail(CIList* lst, CIListNode* next,ConcertInstrument *CInstrument) {

	CIListNode* newNode = (CIListNode*)malloc(sizeof(CIListNode));
	chekeAlloction(newNode);

	newNode->next = next;
	newNode->CInstrument.importance = CInstrument->importance;
	newNode->CInstrument.inst = CInstrument->inst;
	newNode->CInstrument.num = CInstrument->num;

	insertNodeToCIListTail(lst, newNode);

}

void arrangeArrayAppropriateToTheImportance(CIList lst, Musician*** musicianCollection,int *arrOfSize) {

	CIListNode* cur;
	cur = lst.head;
	int id;

	while (cur != NULL) {

		id = cur->CInstrument.inst;
		
		if (cur->CInstrument.importance == 1) {

			sortArrayMusicianOfThisIDInstrumentToLower(musicianCollection[id],arrOfSize[id], id);
		}else {

			sortArrayMusicianOfThisIDInstrumentToHigher(musicianCollection[id], arrOfSize[id], id);
		}
		cur = cur->next;
	}
}

void sortArrayMusicianOfThisIDInstrumentToLower(Musician **musician,int size,int  id) {

	ListNode* cur,*cur2;
	int i ,j;
	int price,price2;

	for (i = 0; i < size-1; i++) {

		for (j = 0; j < size - i-1; j++) {

			cur = musician[j]->instrument.head;
			cur2 = musician[j + 1]->instrument.head;

			while (cur != NULL) {

				if (cur->mpi.insID == id) {
					price = cur->mpi.price;
				}

				cur = cur->next;
			}
			while (cur2 != NULL) {

				if (cur2->mpi.insID == id) {
					price2 = cur2->mpi.price;
				}

				cur2 = cur2->next;
			}
			if (price <= price2) {
				swap(&(musician[j]), &(musician[j + 1]));
			}
		}
	}
}

void sortArrayMusicianOfThisIDInstrumentToHigher(Musician** musician, int size, int  id) {

	ListNode* cur, * cur2;
	int i, j;
	int price, price2;

	for (i = 0; i < size - 1; i++) {

		for (j = 0; j < size - i - 1; j++) {

			cur = musician[j]->instrument.head;
			cur2 = musician[j + 1]->instrument.head;

			while (cur != NULL) {

				if (cur->mpi.insID == id)
					price = cur->mpi.price;

				cur = cur->next;
			}
			while (cur2 != NULL) {

				if (cur2->mpi.insID == id)
					price2 = cur2->mpi.price;

				cur2 = cur2->next;
			}
			if (price >= price2) {
				swap(&(musician[j]), &(musician[j + 1]));
			}
		}
	}
}


void swap(Musician** a, Musician** b) {
	
	Musician* tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}


void printCIList(CIList lst) {

	CIListNode *cur;
	cur = lst.head;
	while (cur != NULL) {

		printf("%d ", cur->CInstrument.importance);
		printf("%d ", cur->CInstrument.num);
		printf("%d ", cur->CInstrument.inst);

		cur = cur->next;
	}

}

//purpose :
//to put every musician in a new array collection 
//go over evry the array for every instrumnet and put my musicain in the new array colection
//arrOfsizeWMP-meaning the array of num of musician that playing  an instrument in the concert
Musician*** arrangeMusicianToConcert(CIList lst, Musician*** musicianCollection, int* arrOfSize,int *lsize,int *arrOfsizeWMP) {

	CIListNode* cur;
	cur = lst.head;
	int id,psize;
	psize = 2;
	Musician** musicianplayinconcert;
	Musician*** allMusicianWhoPlayInConcert=(Musician***)malloc(sizeof(Musician**)*psize);

	while (cur != NULL) {

		id = cur->CInstrument.inst;
		if (*lsize == psize) {
			psize = psize * 2;
			allMusicianWhoPlayInConcert = (Musician***)realloc(allMusicianWhoPlayInConcert, sizeof(Musician**) * psize);
		}
		//this function will get the the musician who will play in the concert
		//for every instrument with go over the array collection 
		//return the musician array that was not chosen
		//musiciancollection[id]-this is the musician who playing with the instrument that have an this ID ont the NLSIT
		musicianplayinconcert=getMusicianWhoWillPlayInConcert(musicianCollection[id], arrOfSize[id],cur->CInstrument.num);

		if (musicianplayinconcert == NULL) {
			//this function make the choosen musician 0 and free the array because the concert not going on.
			makeZeroOnChoosenMusician(allMusicianWhoPlayInConcert, *lsize, arrOfsizeWMP);

			return NULL;

		}else {

			allMusicianWhoPlayInConcert[*lsize] = musicianplayinconcert;
			(*lsize)++;
		}
		cur = cur->next;
	}

	allMusicianWhoPlayInConcert = (Musician***)realloc(allMusicianWhoPlayInConcert, sizeof(Musician**) * (*lsize));
	return allMusicianWhoPlayInConcert;
}

//purpose: to put the musician in a new array musician who will play in the concert
//if ther are not enough musician for my concert reutnr null if yes return the array
//musician-the musician that playing an instrument
//size-the size array of this musician array
//num-the amount of how many i need for my concert
Musician** getMusicianWhoWillPlayInConcert(Musician** musician, int size,int num) {

	ListNode* cur;
	int j = 0, counter = 0;
	Musician** musicianplayinconcert = (Musician**)malloc(sizeof(Musician*) * num);

	while (counter != num && j < size) {

		if (musician[j]->chozen == 0) {

			musician[j]->chozen = 1;
			musicianplayinconcert[counter] = musician[j];
			counter++;
		}
		j++;
	}

	if (counter == num) {

		return musicianplayinconcert;

	}else {
		j--;
		while (counter != 0 && 0 < j) {

			if (musician[j]->chozen == 1) {

				musician[j]->chozen = 0;
				counter--;
			}
			j--;
		}
		free(musicianplayinconcert);
		return NULL;
	}
}

int* getArrOfSizeOfEvryInstrument(CIList lst) {

	CIListNode* cur;
	cur = lst.head;
	int lsize = 0;
	int psize = 2;

	int* arrOfsizeOfEvryInstrument = (int*)malloc(sizeof(int) * psize);

	while (cur != NULL) {

		if (psize == lsize) {
			psize = psize * 2;
			arrOfsizeOfEvryInstrument = (int*)realloc(arrOfsizeOfEvryInstrument, sizeof(int*) * psize);
		}
		arrOfsizeOfEvryInstrument[lsize] = cur->CInstrument.num;
		lsize++;
		cur = cur->next;
	}

	return arrOfsizeOfEvryInstrument;
}

//purpose : to make the chosen zero for the musician that successe to be save in the concert
//but the concert not have enough musician so i need to "arrange back" the musician
void makeZeroOnChoosenMusician(Musician*** allMusicianWhoPlayInConcert, int size, int* arrSize) {

	int i, j;

	for (i = 0; i < size; i++) {

		for (j = 0; j < arrSize[i]; j++) {
			allMusicianWhoPlayInConcert[i][j]->chozen = 0;
		}
		free(allMusicianWhoPlayInConcert[i]);
	}
	free(allMusicianWhoPlayInConcert);
}

void printError(char* name) {

	printf("Could not find musicians for the concert %s \n", name);

}

//function that is purpose is print the information.
//the  function get :
//name-nameofconcert ____allMusician-array of musician who play in the concert________size-size of th array allMusician
//date-the day,hour,year,min to print.
//arrofsize-an array with the different size of array in the array all musician colection
//nameofinstrument-an array of the names of the instrument that musician use in this concert
void printTheConcert(Musician*** allMusician,int size, char* name, Date *date,int *arrOfsizeOfMusicianWhoPlay,IDName* arrInstrument) {
	 
	int totalePrice=0;
	//totaleprice=getTheToTalPrice(allMusician)
	printf("the concert : %s \n", name);
	printf("at this time : %d \ %d \%d %f : %f \n", date->day, date->month, date->year, date->hour, date->min);
	printArrayCollection(allMusician, size, arrOfsizeOfMusicianWhoPlay, arrInstrument,&totalePrice);
	printf("the total Price for this cocncert is : %d \n", totalePrice);
}

void printArrayCollection(Musician*** musicianCollection, int numOfInstrument, int* arrOfsizeOfMusicianWhoPlay, IDName* arrInstrument,int *totalPrice) {

	int i;

	for (i = 0; i < numOfInstrument; i++) {

		printf("the name of the instrument: %s \n", arrInstrument[i].nameOfInstrument);
		printArrayOfMusician(musicianCollection[i], arrOfsizeOfMusicianWhoPlay[i],arrInstrument[i].idOfInstrument,totalPrice);

	}
}

void printArrayOfMusician(Musician** musicianGroup, int size,int id,int *totalPrice) {

	int i;
	
	for (i = 0; i < size; i++) {

		printName(musicianGroup[i]->lenghtName, musicianGroup[i]->name);
		printPrice(musicianGroup[i]->instrument,id,totalPrice);

	}

}

void printPrice(MPIList lst, int id,int *totalPrice) {


	ListNode* cur;
	cur = lst.head;

	while (cur != NULL) {

		if (cur->mpi.insID==id) {

			printf("the price of the instrument is :%f \n", cur->mpi.price);
			*totalPrice = *totalPrice + cur->mpi.price;
		}

		cur = cur->next;
	}
}


void printName(int legnthName,char **name) {

	int i;

	printf("the musician : \n");

	for (i = 0; i < legnthName; i++) {

		printf(" %s ",name[i]);

	}
	printf("\n");
}