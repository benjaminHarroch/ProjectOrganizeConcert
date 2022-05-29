#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAX_INSTRUMENT_NAME 150
#define NOT_FOUND -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


///******************************************************************************////
//               ALL MY HELPER STRUCTUR IN THE PROJECT                           ////

typedef struct treeNode {

	char* instrument;
	unsigned short insID;
	struct treeNode* left;
	struct treeNode* right;

} TreeNode;


typedef struct tree {

	TreeNode* root;

} InstrumentTree;

typedef struct musicianpriceinstrument {
    
	unsigned short insID;
	float price;

} MusicianPriceInstrument;


typedef struct node {

	MusicianPriceInstrument mpi;
	struct ListNode* next;

}ListNode;

typedef struct list {

	ListNode* head;
	ListNode* tail;

}MPIList;

typedef struct musician {
	
	int lenghtName;
	char** name;
	int chozen;
	 MPIList instrument;

} Musician;


typedef struct date {
	int day, month, year;
	float hour;
	float min;
} Date;

typedef struct concertInstrument{
	int num;
	int inst;
	char importance;

} ConcertInstrument;

typedef struct cilistNode {

	ConcertInstrument CInstrument;
	struct CIListNode* next;

}CIListNode;

typedef struct cilist {

	CIListNode* head;
	CIListNode* tail;

}CIList;

typedef struct concert {
	Date date_of_concert;
	char* name;
	CIList instrument;
}CONCERT;

typedef struct IdandName {

	char* nameOfInstrument;
	int idOfInstrument;

}IDName;


///***************************************************************************************///////
//                    ALL MY FUNCTIONS NAME SIGNATURE IN THE PROJECT                      ///////

void buildBinaryTreeFromFile(char* filename, InstrumentTree* tr);
TreeNode* creatTreeNode(char* instrument, unsigned short *insID, TreeNode* left, TreeNode* right);
char* getTheInstrument(FILE* fp);
void chekeIfOpen(FILE* fp);
void chekeAlloction(void* ptr);
void helperBuild(TreeNode* root, char* instrument, int* flage, int* id);
void freeTree(InstrumentTree *tr);
void  FreeTreehelper(TreeNode* root);
int findInsId(InstrumentTree tree, char* instrument);
void helperFindInsId(TreeNode* root, char* instrument, int* flage, int* id);
void saveMusicianFromFile(char* filename, InstrumentTree* tr);
void makeEmptyList(MPIList* lst);
void insertNodeToTail(MPIList* lst, ListNode* cur);
void creatNodeAndInsertToTail(MPIList* lst, ListNode* next, unsigned short insID, float price);
void printList(MPIList lst);
char** buildNameAndList(Musician*** musicianGroup, InstrumentTree* tr, int lsizeForMusicianArray,char *line, int* lsizeName);
void buildArrayOfMusicianCollection(Musician** musicianGroup, InstrumentTree* tr, int size);
int  getNumOfInstrument(InstrumentTree* tr);
void helperGetNumsOfInstrument(TreeNode* root, int* counter);
Musician** getArrayOfMusicianWhoPlayTheSameInstrument(Musician** musicianGroup, int id, int size,int *arrOfSize);
Musician* getTheMusicianWhoPlayTheSameInstrument(Musician** musicianGroup, int index, int id);
void getConcertFromTheUserAndShowConcert(InstrumentTree tr,Musician ***musicianCollection,int numOfInstrument,int *arrOfSize);
char* getStringe();
IDName* getListOfInstrumentInTheConcert(char* str, InstrumentTree tr);
void makeEmptyCIList(CIList* lst);
void insertNodeToCIListTail(CIList* lst, CIListNode* cur);
void creatNodeAndInsertToCIListTail(CIList* lst, CIListNode* next, ConcertInstrument* CInstrument);
void arrangeArrayAppropriateToTheImportance(CIList lst, Musician*** musicianCollection);
void sortArrayMusicianOfThisIDInstrumentToHigher(Musician** musician, int size, int  id, int* arrhelper);
void sortArrayMusicianOfThisIDInstrumentToLower(Musician** musician, int size, int  id, int* arrhelper);
void swap(Musician** a, Musician** b);
Musician*** arrangeMusicianToConcert(CIList lst, Musician*** musicianCollection, int* arrOfSize, int* lsize, int* arrOfsizeWMP);
Musician** getMusicianWhoWillPlayInConcert(Musician** musician, int size, int num);
int* getArrOfSizeOfEvryInstrument(CIList lst);
void makeZeroOnChoosenMusician(Musician*** allMusicianWhoPlayInConcert, int size, int* arrSize);
void printError(char* name);
void printTheConcert(Musician*** allMusician, int size, char* name, Date* date, int* arrofsize, IDName* arrInstrument);
void printArrayCollection(Musician*** musicianCollection, int numOfInstrument, int* arrofsize, IDName* arrInstrument, int* totalPrice);
void printArrayOfMusician(Musician** musicianGroup, int size, int id, int* totalPrice, char* nameInstrument);
void printPrice(MPIList lst, int id, int* totalPrice, char* nameInstrument, char **name,int lenName);
void printName(int legnthName, char** name);
void copySalaryToArr(Musician** musician, int* arrhelper, int size, int id);
void copySortingSalaryToMusicianArr(Musician** musician, int* arrhelper, int size, int id);
int cmpToLower(void* a, void* b);
int cmpToHigher(void* a, void* b);
void freeData(int* arrOfsize, Musician*** musicianCollection, int sizeArrayCollection, Date* date,int sizeArray, IDName* arrInstrument);
void freeARRIDName(IDName* arrInstrument, int sizeArray);
void freeMusicianCollection(Musician*** musicianCollection, int* arrOfsize, int sizeArray);
void freeCIlist(CIList lst);
void freeMPIlist(MPIList lst);