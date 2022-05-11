
#include "Header.h"


void main(int argc, char** argv) {

	InstrumentTree tr;
	CONCERT concert;

	buildBinaryTreeFromFile(argv[1],&tr);
	printTree(tr);
	printf("\n");
	saveMusicianFromFile(argv[2],&tr);

}