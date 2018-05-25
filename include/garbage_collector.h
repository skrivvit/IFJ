#ifndef _GARBAGE_COLLECTOR
#define _GARBAGE_COLLECTOR

#include <stdio.h>
#include <malloc.h>



// Polozka garbage collectoru
typedef struct garbageItem{

	void *data;						// Data
	struct garbageItem *ptr;		// Ukazatel na dalsi polozku
}*gItem;


// Seznam polozek garbage collectoru
typedef struct garbageList{

	struct garbageItem *first;		// Ukazatel na prvni polozku seznamu
}gList;

// Deklarace funkci pro praci s garbage collectorem

void gListInit();
void *gMalloc(int size);
void *gReAlloc(void *ptr, int size);	// Funkce pro praci s pameti
void gFree();

void updateList(void *memory, int size, void *memoryToFree);				// pomocne funkce
void cancelPtr(void *memoryToFree);

extern int result;

#endif
