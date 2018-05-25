#include "ilist.h"


void instListInit(tInstList *list){     //inicializace seznamu
	list->Active = NULL;
	list->First = NULL;
	list->Last = NULL;
}

void instListSucc(tInstList *list){     //posouva aktivitu na dalsi instrukci
	if (list->Active == NULL){
		list->Active = list->First;
	}
	else
	list->Active = list->Active->ptr;
}

void instListInsert(tInstList *list, tInst inst){   // vklada prvek a konec seznamu
	tItem *tmp;

	if ((tmp = gMalloc(sizeof(struct Item))) == NULL){
		result = INT_ERR;
		return;
	}
	tmp->instruction = inst;
	tmp->ptr = NULL;

	if (list->First == NULL){
		list->First = tmp;
	}
	else{
		list->Last->ptr = tmp;
	}
	list->Last = tmp;
}

tInst createInst(tInstType type, void* addr1, void* addr2, void* addr3){
	tInst item;
	item.type = type;
	item.addr1 = addr1;
	item.addr2 = addr2;
	item.addr3 = addr3;

	return item;
}
