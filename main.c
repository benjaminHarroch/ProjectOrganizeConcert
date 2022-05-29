//**********///
//ID :209877729
//**********///

#include "Header.h"


void main(int argc, char** argv) {

	InstrumentTree tr;
	CONCERT concert;

	buildBinaryTreeFromFile(argv[1],&tr);
	saveMusicianFromFile(argv[2],&tr);
	freeTree(&tr);
}