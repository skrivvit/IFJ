#include "scanner.h"
#include "garbage_collector.h"
#include "parser.h"
#include "ial.h"
#include "precedence.h"
#include "ilist.h"
#include <stdlib.h>

	FILE *file;		// soubor globalne

int main(int argc, char** argv){

	int result = SYNTAX_OK;

	if (argc != 2){
		return 99;
	}


	if ((file = fopen(argv[1], "r")) == NULL){
		return INT_ERR;
	}

		symbolTablePtr symbolTable;
		BTInit(&symbolTable);

		result = parse(&symbolTable);

	gFree();

	fclose(file);

	return result;
}
