#ifndef _IAL
#define _IAL

#include "garbage_collector.h"
#include "ilist.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BT_OK 0
#define BT_ERR 1

struct BTree;

typedef union {
	int i;
	double r;
	char *s;
	bool b;

}sValue;

typedef enum{
	tInt,
	tString,
	tBool,
	tReal
}sType;

typedef struct{
	char *name;
	sType type;
}sParam;


typedef struct params{
	sParam param;
	struct params *ptr;
}*sParams;


typedef struct Symbol{
	char *key;
	sType type;
	sParams params;
	sValue value;
	bool isFunction;
	bool isDefined;
	struct BTree *symbolTable;		// lokalni tabulka symbolu pro funkce
	tInstList lInstList;

}symbol, *symbolPtr;


typedef struct BTree{
	char *key;
	symbol content;
	struct BTree *LPtr;
	struct BTree *RPtr;

}*symbolTablePtr;

int BTInsert_(symbolTablePtr *root, char *key, symbol s);
symbolTablePtr BTSearch_(symbolTablePtr *root, char *key);
void STInit(symbolTablePtr *root);
void BTInit(symbolTablePtr *root);
int BTInsert(symbolTablePtr *root, char *key, symbol s);
symbolTablePtr BTSearch(symbolTablePtr *root, char *key);
void BTPrint(symbolTablePtr *root);
extern symbolTablePtr func;
extern int function;

int length(char *str);
char *copy(char* str, int position, int count);
void prefix(char* s, int search_size, int *arr);
int find(char *s, char *search);
void Merge(char *array, int left, int mid, int right);
void MergeSort(char *array, int left, int right);
char* sort(char * s);

#endif