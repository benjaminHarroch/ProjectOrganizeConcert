

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
	
	//after building the tree can left and free the instrumet
	free(instrument);
	fclose(fp);
}

TreeNode* creatTreeNode(char *instrument, unsigned short *insID, TreeNode *left, TreeNode *right) {

	int size;
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	chekeAlloction(newNode);

	newNode->insID = *insID;
	size = strlen(instrument);
	newNode->instrument = (char*)malloc(size+1);
	chekeAlloction(newNode->instrument);
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
	chekeAlloction(instrument);
	
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

void freeTree(InstrumentTree *tr) {

	FreeTreehelper(tr->root);
}

void  FreeTreehelper(TreeNode* root) {

	if (root == NULL) {
		return;
	}
	else {

		FreeTreehelper(root->left);
		FreeTreehelper(root->right);
//		free(root->instrument);
		free(root);
	}
}

//purpose: search the ID of an instrument
//if not find return NULL if find return the ID
int findInsId(InstrumentTree tree, char* instrument){

	int flage, id;
	flage = 0;
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
	chekeAlloction(musicianGroup);

	ptr = fgets(line, 150, fp);

	//this while run on every line in the file per one line every time
	while (ptr != NULL) {

		if (lsizeForMusicianArray == psizeForMusicianArray) {
			psizeForMusicianArray = psizeForMusicianArray * 2;
			musicianGroup = (Musician**)realloc(musicianGroup, psizeForMusicianArray * (sizeof(Musician*)));
			chekeAlloction(musicianGroup);
		}

		musicianGroup[lsizeForMusicianArray] = (Musician*)malloc(sizeof(Musician));
		chekeAlloction(musicianGroup[lsizeForMusicianArray]);
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

////////////////////////////////////////////////////////////////////////////////////////////
  /////////// FUNCTION HELP MPILIST : LINKED LIST (MAKE EMPTY-INSERTNODE-CREAT NODE)//////////
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

//******************************************************************************************************
//******************************************************************************************************

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
				chekeAlloction(name);
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
	chekeAlloction(arrOfSize);
	chekeAlloction(musicianCollection);

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
	chekeAlloction(musicianGroupofTheSameInstrument);

	//loop that run one the musician group for search the ID on the array
	//every one who have this ID one this array so a get him for my new array
	for (i = 0; i < size; i++) {

		Musician* tmp;

		if (lsizeForMusicianArray == psizeForMusicianArray) {
			psizeForMusicianArray = psizeForMusicianArray * 2;
			musicianGroupofTheSameInstrument = (Musician**)realloc(musicianGroupofTheSameInstrument, psizeForMusicianArray * (sizeof(Musician*)));
			chekeAlloction(musicianGroupofTheSameInstrument);
		}


		 tmp=getTheMusicianWhoPlayTheSameInstrument(musicianGroup, i,id);
		
		 //if i found a instrument so add to array and add one place else do nothing
		 if (tmp != NULL) {
			 musicianGroupofTheSameInstrument[lsizeForMusicianArray] = tmp;
			 lsizeForMusicianArray++;
		 }
	} 
	musicianGroupofTheSameInstrument = (Musician**)realloc(musicianGroupofTheSameInstrument, lsizeForMusicianArray * (sizeof(Musician*)));
	chekeAlloction(musicianGroupofTheSameInstrument);
	arrofsize[id] = lsizeForMusicianArray;

	return musicianGroupofTheSameInstrument;
}

// search for me on the array in place index over the list if the ID exist
//if yes send me back this musician 
Musician* getTheMusicianWhoPlayTheSameInstrument(Musician** musicianGroup,int index, int id) {
	
	MPIList lst = musicianGroup[index]->instrument;
	ListNode* cur = lst.head;
	Musician *musicianGroupofTheSameInstrument = (Musician*)malloc(sizeof(Musician));
	chekeAlloction(musicianGroupofTheSameInstrument);

	while (cur != NULL) {

		if (cur->mpi.insID == id) {
			
			musicianGroupofTheSameInstrument = musicianGroup[index];
			return musicianGroupofTheSameInstrument;
		}
		cur = cur->next;
	}
	//free the data because he dont find the ID in the list
	free(musicianGroupofTheSameInstrument);
	return NULL;
}

//arrofsize-this is an array that save my size of every array 
//example - arrofsize[0] save the size of array musician in place 0 of array collection
void getConcertFromTheUserAndShowConcert(InstrumentTree tr, Musician*** musicianCollection, int numOfInstrument, int* arrOfSize) {
	int lenghtLine, saveSizeArray = 0, sizeArray = 0;
	int* arrOfsizeOfMusicianWhoPlay = NULL;
	char* str, * name;
	IDName* arrInstrument = NULL;
	Date* date;
	date = (Date*)malloc(sizeof(Date));
	chekeAlloction(date);
	str = getStringe();
	if (str != NULL) {

		lenghtLine = strlen(str);
		CIList lst;
		Musician*** allMusicianWhoPlayInConcert = NULL;

		while (str != NULL) {

			lenghtLine = strlen(str);
			name = (char*)malloc(sizeof(char) * lenghtLine);
			chekeAlloction(name);

			sscanf(str, "%s %d %d %d %f:%f", name, &date->day, &date->month, &date->year, &date->hour, &date->min);
			lenghtLine = strlen(name);
			name = (char*)realloc(name, lenghtLine + 1);
			chekeAlloction(name);
			name[lenghtLine + 1] = '\0';

			arrInstrument = getListOfInstrumentInTheConcert(str, tr, &lst);
			arrangeArrayAppropriateToTheImportance(lst, musicianCollection, arrOfSize);

			//array whit number of every musician i need to a concert for veri instrument
			arrOfsizeOfMusicianWhoPlay = getArrOfSizeOfEvryInstrument(lst);

			allMusicianWhoPlayInConcert = arrangeMusicianToConcert(lst, musicianCollection, arrOfSize, &sizeArray, arrOfsizeOfMusicianWhoPlay);

			if (allMusicianWhoPlayInConcert == NULL) {
				printError(name);
			}
			else {
				//allmusicianwhoplayinconcert- this is the new array collection whit the musician that playing in the concert
			   //sizearray- is my size of this array 
				printTheConcert(allMusicianWhoPlayInConcert, sizeArray, name, date, arrOfsizeOfMusicianWhoPlay, arrInstrument);
			}

			saveSizeArray = sizeArray;
			sizeArray = 0;
			free(str);
			freeCIlist(lst);
			//free the arr IDName structur 
			freeARRIDName(arrInstrument, saveSizeArray);
			str = getStringe();
		}
	}
	freeData(arrOfSize, musicianCollection, numOfInstrument, date, saveSizeArray, arrInstrument);
}



char* getStringe() {
	char* str;
	int logSize = 0, phySize = 1, flage = 0;
	char c;
	printf(" Please enter the Concert woul you like to cheke : \n ");
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
	chekeAlloction(str);
	str[logSize] = '\0';
	flage=chekeIfLineBlanc(str);
	if (flage == 0) {
		free(str);
		return NULL;
	}

	return str;
}

int chekeIfLineBlanc(char* str) {
	 
	int flage = 1;

	if (strlen(str) == 0) {
		flage = 0;
		return flage;

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
				chekeAlloction(arrInstrument);
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
	chekeAlloction(arrInstrument);
	*alst = lst;
	return arrInstrument;
}

////////////////***********************************////////////////////////////////////////////
///            FUNTION HELP CILIST : LIKEND LIST (MAKE EMPTY ,INSER NODE,CREATE NODE //////////
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

////////////////************************////////////////////////////////////////////////
///***************************************************************************//////////


//function for arrange the array accordinate to the importance
//if the importance is 1 so arrange the array of musician from high to low if 0 from low to zero
//use an array of salary for help the qsort .
void arrangeArrayAppropriateToTheImportance(CIList lst, Musician*** musicianCollection,int *arrOfSize) {

	CIListNode* cur;
	cur = lst.head;
	int id;
	

	while (cur != NULL) {

		id = cur->CInstrument.inst;

		int* arrhelper = (int*)malloc(sizeof(int) * arrOfSize[id]);
		copySalaryToArr(musicianCollection[id], arrhelper, arrOfSize[id], id);

		if (cur->CInstrument.importance == 1) {

			sortArrayMusicianOfThisIDInstrumentToLower(musicianCollection[id],arrOfSize[id], id,arrhelper);
		}else {

			sortArrayMusicianOfThisIDInstrumentToHigher(musicianCollection[id], arrOfSize[id], id,arrhelper);
		}
		cur = cur->next;
	}
}


//sorting the array salary of an ID musician 
//sort with qsort
//after sorting -> arrange the musician ID accordinate to the order of the salary array
void sortArrayMusicianOfThisIDInstrumentToLower(Musician **musician,int size,int  id,int *arrhelper) {

	qsort(arrhelper,size,sizeof(int),cmpToLower);
	copySortingSalaryToMusicianArr(musician, arrhelper, size,id);
	free(arrhelper);

}

//function compare for the qsort function
int cmpToLower(void *a ,void *b) {

	int* firstSalary = (int*)a;
	int* secondSalary = (int*)b;

	return ( (*secondSalary)- (*firstSalary));
}

//function compare for the qsort function this function return a num if the nume is positive so the first num is 
//higher then the second in the array.
int cmpToHigher(void* a, void* b) {

	int* firstSalary = (int*)a;
	int* secondSalary = (int*)b;

	return ((*firstSalary) - (*secondSalary));
}

//sorting the array salary of an ID musician 
//sort with qsort
//after sorting -> arrange the musician ID accordinate to the order of the salary array
void sortArrayMusicianOfThisIDInstrumentToHigher(Musician** musician, int size, int  id ,int *arrhelper) {

	qsort(arrhelper, size, sizeof(int), cmpToHigher);
	copySortingSalaryToMusicianArr(musician, arrhelper, size, id);
	free(arrhelper);
}

//simple function swap with pointer swaping
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
			chekeAlloction(allMusicianWhoPlayInConcert);
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
	
	//this function make zero the choosen because i finish to chooze my musician but not free the arrray
	makeZeroOnChoosenMusician(allMusicianWhoPlayInConcert, *lsize, arrOfsizeWMP);
	allMusicianWhoPlayInConcert = (Musician***)realloc(allMusicianWhoPlayInConcert, sizeof(Musician**) * (*lsize));
	chekeAlloction(allMusicianWhoPlayInConcert);
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
	chekeAlloction(musicianplayinconcert);

	while (counter != num && j < size) {

		if (musician[j]->chozen == 0) {

			musician[j]->chozen = 1;
			//theye point to the same pointer so i need be carfule whene i free my arr (musiciainplayinconcert)
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

//function puprose - a function for build an array that save me the number of every musician i need of 
//one instrument in the concert
int* getArrOfSizeOfEvryInstrument(CIList lst) {

	CIListNode* cur;
	cur = lst.head;
	int lsize = 0;
	int psize = 2;

	int* arrOfsizeOfEvryInstrument = (int*)malloc(sizeof(int) * psize);
	chekeAlloction(arrOfsizeOfEvryInstrument);

	while (cur != NULL) {

		if (psize == lsize) {
			psize = psize * 2;
			arrOfsizeOfEvryInstrument = (int*)realloc(arrOfsizeOfEvryInstrument, sizeof(int*) * psize);
			chekeAlloction(arrOfsizeOfEvryInstrument);
		}
		arrOfsizeOfEvryInstrument[lsize] = cur->CInstrument.num;
		lsize++;
		cur = cur->next;
	}
	arrOfsizeOfEvryInstrument = (int*)realloc(arrOfsizeOfEvryInstrument, sizeof(int*) * lsize);
	chekeAlloction(arrOfsizeOfEvryInstrument);
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
	}
}

//function puprose - copy the salary to the arr
//this arr will help me for the sorting challenge
void copySalaryToArr(Musician**  musician,int *arrhelper,int size,int id) {

	ListNode* cur;
	int i;


	for (i = 0; i < size; i++) {

		cur = musician[i]->instrument.head;

		while (cur != NULL) {

			if (cur->mpi.insID == id) {

				arrhelper[i] = cur->mpi.price;
			}

			cur = cur->next;
		}

	}
}

//function purpose - function that get an sorting array with the salary of an ID instrument
//and accordinate to the array salary I sort my musician array
void copySortingSalaryToMusicianArr(Musician**  musician, int* arrhelper, int size, int id ) {

	ListNode* cur;
	int i=0,j=0;


	while (i < size) {

		cur = musician[j]->instrument.head;

		while (cur != NULL) {

			if (cur->mpi.insID == id && cur->mpi.price == arrhelper[i]) {

				swap(&musician[i] ,&musician[j]);
				i++;
				j = -1;
			}
			cur = cur->next;
		}
		j++;
	}


}


////////////////////////////////////////////////////////////////////////////////////////////////
//*******************   FUNCTION FOR PRINTING DATA **********************************///////////
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

	printf("*********************************************************************** \n");
	printf("*****             THE CONCERT : %s                             ****** \n", name);
	if (date->min >= 0 && date->min <= 9) {
		printf("*****          TIME OF THE CONCERT : %d/%d/%d  %.f:0%.f           ****** \n", date->day, date->month, date->year, date->hour, date->min);
	}
	else {
		printf("*****          TIME OF THE CONCERT : %d/%d/%d  %.f:%.f           ****** \n", date->day, date->month, date->year, date->hour, date->min);
	}
	printf("*********************************************************************** \n");
	printArrayCollection(allMusician, size, arrOfsizeOfMusicianWhoPlay, arrInstrument,&totalePrice);

	printf("  The total Price for this concert is : %d \n", totalePrice);
	printf("\n");
}

void printArrayCollection(Musician*** musicianCollection, int numOfInstrument, int* arrOfsizeOfMusicianWhoPlay, IDName* arrInstrument,int *totalPrice) {

	int i;

	for (i = 0; i < numOfInstrument; i++) {

		printf(" The musician who will play on : %s \n", arrInstrument[i].nameOfInstrument);
		printArrayOfMusician(musicianCollection[i], arrOfsizeOfMusicianWhoPlay[i],arrInstrument[i].idOfInstrument,totalPrice, arrInstrument[i].nameOfInstrument);

	}
}

void printArrayOfMusician(Musician** musicianGroup, int size,int id,int *totalPrice,char *nameInstrument) {

	int i;
	
	for (i = 0; i < size; i++) {

		printName(musicianGroup[i]->lenghtName, musicianGroup[i]->name);
		printPrice(musicianGroup[i]->instrument,id,totalPrice,nameInstrument, musicianGroup[i]->name, musicianGroup[i]->lenghtName);

	}

}

void printPrice(MPIList lst, int id,int *totalPrice, char* nameInstrument,char** name,int lenName) {


	ListNode* cur;
	cur = lst.head;

	while (cur != NULL) {

		if (cur->mpi.insID==id) {

			printf(" The price for that ");
			printName(lenName, name);
			printf(" play on the %s is :%f \n", nameInstrument, cur->mpi.price);
			*totalPrice = *totalPrice + cur->mpi.price;
		}

		cur = cur->next;
	}
}


void printName(int legnthName,char **name) {

	int i;

	printf(" The musician :  ");

	for (i = 0; i < legnthName; i++) {

		printf(" %s ",name[i]);

	}
	printf("\n");
	printf("\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
//********************************* FREE FUNCTIONS    *******************************///////////

void freeData(int* arrOfsize, Musician*** MusicianCollection,int sizeArrayCollcetion, Date* date,int sizeArray, IDName* arrInstrument) {

	//free the date of the concert 
	free(date);

	//free the musician who was choosen in the previos concert
	freeMusicianCollection(MusicianCollection, arrOfsize, sizeArrayCollcetion);

}

//free the arr IDName structur 
void freeARRIDName(IDName* arrInstrument, int sizeArray) {
	int i;

	for (i = 0; i < sizeArray; i++) {

		free(arrInstrument[i].nameOfInstrument);
	}
	free(arrInstrument);

}

//free the musician who was choosen in the previos concert
void freeMusicianCollection(Musician ***musicianCollection,int* arrOfsize,int sizeArray) {

	int i,j;

	for (i = 0; i < sizeArray; i++) {

		for (j = 0; j < arrOfsize[i] && musicianCollection[i][j]->chozen!= 2; j++) {

			//free the MPIlist 
			freeMPIlist(musicianCollection[i][j]->instrument);

			free(musicianCollection[i][j]->name);

			musicianCollection[i][j]->chozen = 2;

		}
		free(musicianCollection[i]);
	}
	//free the array collection and he size array
	free(musicianCollection);
	free(arrOfsize);
}

//free list of the instrument  in the concert
void freeCIlist(CIList lst) {

	CIListNode* cur,* next;
	cur = lst.head;

	while (cur != NULL) {

		next = cur->next;
		free(cur);
		cur = next;
	}

}

//free list of the instrument  in the concert
void freeMPIlist(MPIList lst) {

	ListNode* cur,*next;
	cur = lst.head;

	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur =next;
	}

}