#ifndef _ILIST
#define _ILIST

#include <stdio.h>
#include "garbage_collector.h"
#include "tmp.h"


/* vycet typu instrukci (neni komplet zatim --hruby navrh-- ) */
typedef enum{
	I_STOP,

	I_ADD,
	I_SUB,
	I_MUL,
	I_DIV,
	I_CONC,		//konkatenace retezcu

	I_LESSER,
	I_GREATER,
	I_LOE,
	I_GOE,
	I_NOTEQUAL,
	I_EQUAL,

	I_LENGTH,
	I_SORT,
	I_COPY,
	I_FIND,
	I_READLN,
	I_WRITE,

	I_ASSIGN,

	I_WHILE,
	I_LABEL,
	I_GOTO,
	I_IFGOTO,

	I_CALL,


	// a tak dale
}tInstType;


typedef struct {
	tInstType type;
	void *addr1;
	void *addr2;
	void *addr3;
}tInst;

typedef struct Item{
	tInst instruction;
	struct Item *ptr;		//ukazatel na dalsi instrukci
}tItem, *tItemPtr;

typedef struct{
	tItem *First;
	tItem *Active;
	tItem *Last;
}tInstList;


void instListInit(tInstList *list);
void instListSucc(tInstList *list);
void instListInsert(tInstList *list, tInst inst);
tInst createInst(tInstType type, void* addr1, void* addr2, void* addr3);
void instListPrint(tInstList *list);
void instListSkip(tInstList *list);


#endif
